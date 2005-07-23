/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

/* file:        src/osgToy/GlslLintVisitor.cpp
 * author:      Mike Weiblen http://mew.cx/ 2005-07-22
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
*/

#include <osg/Notify>
#include <osgToy/GlslLintVisitor>

///////////////////////////////////////////////////////////////////////////

osgToy::GlslLintVisitor::GlslLintVisitor( osgToy::GlslLint::Options options ) :
    osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
}


osgToy::GlslLintVisitor::~GlslLintVisitor()
{
}


void osgToy::GlslLintVisitor::apply( osg::Node& node )
{
    apply( node.getStateSet() );
    traverse( node );
}


void osgToy::GlslLintVisitor::apply( osg::Geode& geode )
{
    apply( geode.getStateSet() );

    for( unsigned int i = 0; i < geode.getNumDrawables(); i++ )
    {
        apply( geode.getDrawable(i)->getStateSet() );
    }

    traverse( geode );
}


void osgToy::GlslLintVisitor::apply( const osg::StateSet* stateset )
{
    if( !stateset ) return;

    pushState();

    const osg::StateSet::UniformList& uniformList = stateset->getUniformList();
    for( osg::StateSet::UniformList::const_iterator uitr = uniformList.begin();
            uitr != uniformList.end(); uitr++ )
    {
        //const std::string& name = uitr->first;
        //const RefUniformPair& rup = uitr->second;
        //const osg::Uniform* uniform = rup.first.get();
        //osg::StateAttribute::OverrideValue override = rup.second;

        const osg::Uniform* uniform = uitr->second.first.get();
        apply( uniform );
    }

    const osg::Program* program = dynamic_cast<const osg::Program*>(
            stateset->getAttribute( osg::StateAttribute::PROGRAM ));
    apply( program );

    popState();
}

 
void osgToy::GlslLintVisitor::apply( const osg::Program* program )
{
    if( !program ) return;

    const std::string& name = program->getName();
    osg::notify(osg::INFO)
        << "\nProgram \"" << name << "\"" << std::endl;

    const osg::Program::AttribBindingList& attribList = program->getAttribBindingList();
    for( osg::Program::AttribBindingList::const_iterator aitr = attribList.begin();
            aitr != attribList.end(); aitr++ )
    {
        const std::string& name = aitr->first;
        GLuint index = aitr->second;

        //TODO
        osg::notify(osg::INFO)
            << "AttributeBinding \"" << name
            << "\" index=" << index << std::endl;
    }

    unsigned int numShaders = program->getNumShaders();
    for( unsigned int i = 0; i < numShaders; i++ )
    {
        const osg::Shader* shader = program->getShader(i);
        apply( shader );
    }

}

 
void osgToy::GlslLintVisitor::apply( const osg::Shader* shader )
{
    if( !shader ) return;

    const std::string& name = shader->getName();
    const osg::Shader::Type type = shader->getType();
    const std::string& sourceText = shader->getShaderSource();

    //TODO
    osg::notify(osg::INFO)
        << "Shader \"" << name
        << "\" type=" << shader->getTypename()
        << " sourceText=\n" << sourceText << "\n" << std::endl;
}


void osgToy::GlslLintVisitor::apply( const osg::Uniform* uniform )
{
    if( !uniform ) return;

    const std::string& name = uniform->getName();
    const osg::Uniform::Type type = uniform->getType();

    //TODO
    osg::notify(osg::INFO)
        << "Uniform \"" << name
        << "\" type=" << uniform->getTypename(type) << std::endl;
}


void osgToy::GlslLintVisitor::pushState()
{
    //TODO
}


void osgToy::GlslLintVisitor::popState()
{
    //TODO
}

// vim: set sw=4 ts=8 et ic ai:
