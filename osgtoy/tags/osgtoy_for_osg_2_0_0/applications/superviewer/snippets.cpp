// mew 2005-10-13

///////////////////////////////////////////////////////////////////////////
// make penrose triangle endcaps

osg::Node* makePTendcaps()
{
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* vAry = new osg::Vec3Array;
    geom->setVertexArray( vAry );

    osg::Vec4Array* cAry = new osg::Vec4Array;
    geom->setColorArray( cAry );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    cAry->push_back( osg::Vec4(1,1,1,1) );

    osg::Vec3 a0(  0.1,  0.2,  1.2 );
    osg::Vec3 a1(  0.1,  0.2,  0.8 );
    osg::Vec3 a2( -0.1, -0.2,  0.8 );
    osg::Vec3 a3( -0.1, -0.2,  1.2 );

    vAry->push_back( a0 ); vAry->push_back( a1 ); vAry->push_back( a2 );
    vAry->push_back( a2 ); vAry->push_back( a3 ); vAry->push_back( a0 );

    geom->addPrimitiveSet( new osg::DrawArrays( GL_TRIANGLES, 0, vAry->getNumElements() ) );

    osg::Geode* geode = new osg::Geode();
    geode->addDrawable( geom );

    osg::Group* group = new osg::Group;
    group->addChild( geode );

    osg::PositionAttitudeTransform* xform;
    xform = new osg::PositionAttitudeTransform;
    xform->setAttitude( osg::Quat( osg::DegreesToRadians(120.0), osg::Vec3(0,1,0) ) );
    xform->addChild( geode );
    group->addChild( xform );

    xform = new osg::PositionAttitudeTransform;
    xform->setAttitude( osg::Quat( osg::DegreesToRadians(240.0), osg::Vec3(0,1,0) ) );
    xform->addChild( geode );
    group->addChild( xform );

    return group;
}

// vim: set sw=4 ts=8 et ic ai:
