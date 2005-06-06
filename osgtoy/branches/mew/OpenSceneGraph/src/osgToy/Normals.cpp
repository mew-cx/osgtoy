// This code originally developed and posted to the osg-users mailing list
// by Don Burns on 2004-12-10; see:
// http://openscenegraph.org/pipermail/osg-users/2004-December/048243.html
// mew 2004-12-22


#include "osgToy/Normals"

osgToy::Normals::Normals( Node *node, float scale, Mode mode )
{
    MakeNormalsVisitor mnv(scale);
    mnv.setMode( mode );
    node->accept( mnv );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    osg::ref_ptr<osg::Vec3Array> coords = mnv.getCoords();
    geom->setVertexArray( coords.get() );

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    geom->setColorArray( colors.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    if( mode == SurfaceNormals )
        colors->push_back( osg::Vec4( 1, 0, 0, 1 ));
    else if( mode == VertexNormals )
        colors->push_back( osg::Vec4( 0, 1, 0, 1 ));

    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, coords->size()));

    osg::StateSet* sset = geom->getOrCreateStateSet();
    sset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    addDrawable( geom.get() );
}

///////////////////////////////////////////////////////////////////////////

osgToy::Normals::MakeNormalsVisitor::MakeNormalsVisitor( float normalScale, Normals::Mode mode): 
            NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN),
            _normal_scale(normalScale),
            _mode(mode)
{
    _local_coords = new osg::Vec3Array;
}


void osgToy::Normals::MakeNormalsVisitor::apply( Geode &geode )
{
    for( unsigned int i = 0; i < geode.getNumDrawables(); i++ )
    {
        osg::Geometry *geom = dynamic_cast<osg::Geometry *>(geode.getDrawable(i));
        if( geom )
        {
            osg::Vec3Array *coords   = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
            if( coords == 0L )
                continue;

            osg::Vec3Array *normals  = dynamic_cast<osg::Vec3Array*>(geom->getNormalArray());
            if( normals == 0L )
                continue;

            osg::Geometry::AttributeBinding binding = geom->getNormalBinding();
            if( binding == osg::Geometry::BIND_OFF )
                continue;

            if( binding == osg::Geometry::BIND_OVERALL )
            {
                osg::Vec3 v(0,0,0);
                osg::Vec3 n = normals->front();

                osg::Vec3Array::iterator coord_index = coords->begin();
                while( coord_index != coords->end() )
                  v += *(coord_index++);
                v /= (float)(coords->size()); 

                n *= _normal_scale;
                _local_coords->push_back( v );
                _local_coords->push_back( (v + n));
            }
            else // BIND_PER_PRIMTIVE_SET, BIND_PER_PRIMTITIV, BIND_PER_VERTEX
            {
                osg::Geometry::PrimitiveSetList& primitiveSets = geom->getPrimitiveSetList();
                osg::Geometry::PrimitiveSetList::iterator itr;

                osg::Vec3Array::iterator coord_index   = coords->begin();
                osg::Vec3Array::iterator normals_index = normals->begin();

                for(itr=primitiveSets.begin(); itr!=primitiveSets.end(); ++itr)
                {
                    if( binding == osg::Geometry::BIND_PER_PRIMITIVE_SET )
                    {
                        osg::Vec3 v(0,0,0);
                        osg::Vec3 n = *(normals_index++); 
                        int ni = (*itr)->getNumIndices();
                        for( int i = 0; i < ni; i++ )
                            v += *(coord_index++);
                        v /= (float)(ni);

                        n *= _normal_scale;
                        _local_coords->push_back( v );
                        _local_coords->push_back( (v + n));

                    }
                    else 
                    {
                        switch((*itr)->getMode())
                        {
                            case(osg::PrimitiveSet::TRIANGLES):
                                for( unsigned int j = 0; j < (*itr)->getNumPrimitives(); j++ )
                                {
                                    _processPrimitive( 3, coord_index, normals_index, binding );
                                    coord_index += 3;
                                    if( binding == osg::Geometry::BIND_PER_PRIMITIVE )
                                        normals_index++;
                                    else 
                                        normals_index+=3;
                                    
                                }
                                break;

                            case(osg::PrimitiveSet::TRIANGLE_STRIP):
                                for( unsigned int j = 0; j < (*itr)->getNumIndices()-2; j++ )
                                {
                                    _processPrimitive( 3, coord_index, normals_index, binding );
                                    coord_index++;
                                    normals_index++;
                                }
                                coord_index += 2;
                                if( binding == osg::Geometry::BIND_PER_VERTEX )
                                    normals_index += 2;
                                break;

                            case(osg::PrimitiveSet::TRIANGLE_FAN):
                                break;

                            case(osg::PrimitiveSet::QUADS):
                                for( unsigned int j = 0; j < (*itr)->getNumPrimitives(); j++ )
                                {
                                    _processPrimitive( 4, coord_index, normals_index, binding );
                                    coord_index += 4;
                                    if( binding == osg::Geometry::BIND_PER_PRIMITIVE )
                                        normals_index++;
                                    else 
                                        normals_index+=4;
                                }
                                break;

                            case(osg::PrimitiveSet::QUAD_STRIP):
                            case(osg::PrimitiveSet::POLYGON):
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }
    traverse( geode );
}


void osgToy::Normals::MakeNormalsVisitor::_processPrimitive(  unsigned int nv,
                        osg::Vec3Array::iterator coords, 
                        osg::Vec3Array::iterator normals,
                        osg::Geometry::AttributeBinding binding )
{
    osg::Vec3 v(0,0,0);
    osg::Vec3 n(0,0,0);
    if( _mode == SurfaceNormals || binding == osg::Geometry::BIND_PER_PRIMITIVE )
    {
        if( binding == osg::Geometry::BIND_PER_PRIMITIVE )
        {
            n = *(normals++);
        }
        else if( binding == osg::Geometry::BIND_PER_VERTEX )
        {
            for( unsigned int i = 0; i < nv; i++ )
                n += *(normals++);
            n /= (float)(nv); 
        }

        for( unsigned int i = 0; i < nv; i++ )
            v += *(coords++);
        v /= (float)(nv);

        n *= _normal_scale;
        _local_coords->push_back( v );
        _local_coords->push_back( (v + n));
    }
    else if( _mode == VertexNormals )
    {
        for( unsigned int i = 0; i < nv; i++ )
        {
            v = *(coords++);
            n = *(normals++);
            _local_coords->push_back( v );
            _local_coords->push_back( (v + n));
        }
    }
}

/* vim: set sw=4 ts=8 et ic ai: */
/*EOF*/
