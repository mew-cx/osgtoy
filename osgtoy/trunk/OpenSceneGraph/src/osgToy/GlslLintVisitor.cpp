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
 * author:      Mike Weiblen http://mew.cx/ 2005-07-26
 * copyright:   (C) 2005 Michael Weiblen
 * license:     OpenSceneGraph Public License (OSGPL)
*/

#include <iostream>

#include <osgToy/GlslLintVisitor>

///////////////////////////////////////////////////////////////////////////

osgToy::GlslLintVisitor::GlslLintVisitor( osgToy::GlslLint::Options options ) :
    osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
    _options(options)
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

        //FUTURE - at drawable, verify all shader requirements are met.
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

    const osg::Program::AttribBindingList& attribList = program->getAttribBindingList();
    for( osg::Program::AttribBindingList::const_iterator aitr = attribList.begin();
            aitr != attribList.end(); aitr++ )
    {
        const std::string& name = aitr->first;
        GLuint index = aitr->second;

        //FUTURE - populate attribute table for later shader verification.
        std::cout
            << "AttributeBinding \"" << name
            << "\" index=" << index << std::endl;
    }

    std::cout << std::endl;

    osg::ref_ptr<osgToy::GlslLint> glsllint = new osgToy::GlslLint( _options );
    int compileFailures = 0;

    unsigned int numShaders = program->getNumShaders();
    for( unsigned int i = 0; i < numShaders; i++ )
    {
        const osg::Shader* shader = program->getShader(i);
        const osg::Shader::Type type = shader->getType();
        const std::string& sourceText = shader->getShaderSource();

        bool success = (glsllint->compile( type, sourceText ) == osgToy::GlslLint::SUCCESS);

        std::cout
            << "Shader \"" << shader->getName() << "\""
            << " type=" << shader->getTypename()
            // << " sourceText=\n" << sourceText << "\n"
            << " compile=" << ((success) ? "OK" : "FAIL");
        if( !glsllint->getInfoLog().empty() )
            std::cout << " infoLog:\n" << glsllint->getInfoLog();
        std::cout << std::endl;

        if( !success ) compileFailures++;
    }

#if 0 //[
    // see notes in osgToy/GlslLint.cpp

    bool success = (glsllint->link() == osgToy::GlslLint::SUCCESS);

    std::cout
        << "Program \"" << program->getName() << "\""
        << " link=" << ((success) ? "OK" : "FAIL");
    if( !glsllint->getInfoLog().empty() )
        std::cout << " infoLog:\n" << glsllint->getInfoLog();
    std::cout << std::endl;
#else
    bool success = (compileFailures == 0);

    std::cout
        << "Program \"" << program->getName() << "\""
        << " link=" << ((success) ? "OK" : "FAIL");
    if( !success )
        std::cout << " infoLog:\n"
            << "\t" << compileFailures << " of " << program->getNumShaders()
            << " shaders failed compilation";
    std::cout << std::endl;
#endif //]

    std::cout << std::endl;
}

 
void osgToy::GlslLintVisitor::apply( const osg::Uniform* uniform )
{
    if( !uniform ) return;

    const std::string& name = uniform->getName();
    const osg::Uniform::Type type = uniform->getType();

    //FUTURE - populate uniform table for later shader verification.
    std::cout
        << "Uniform \"" << name
        << "\" type=" << uniform->getTypename(type)
        << std::endl;
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
