/* file:        include/osgVRPN/AnalogTracker.h
 * author:      Mike Weiblen
 * copyright:   (C) 2006 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id 2006-07-08 $
*/

#ifndef OSGVRPN_ANALOGTRACKER
#define OSGVRPN_ANALOGTRACKER 1

#include <osg/Vec3>
#include <osg/Matrixd>
#include <osgVRPN/TrackerBase.h>
#include <osgVRPN/Analog.h>
#include <osgVRPN/Button.h>

#include <osgVRPN/Export>

namespace osgVRPN {

/** a TrackerBase which integrates the output of several vrpn_Analog_Remote channels. */

class OSGVRPN_EXPORT AnalogTracker : public TrackerBase
{
public:
    AnalogTracker();

    /** Update our state from the device. */
    void update();

    void reset() {_matrix.makeIdentity();}

    /** Query the tracker's transform matrix */
    osg::Matrixd getMatrix() const {return _matrix;}
    osg::Matrixd getInverseMatrix() const {return osg::Matrixd::inverse(_matrix);}

    void setAnalogDevice( Analog* aDevice ) {_analogDevice = aDevice;}
    void setTranslateChannelX( int chan ) {_chTX = chan;}
    void setTranslateChannelY( int chan ) {_chTY = chan;}
    void setTranslateChannelZ( int chan ) {_chTZ = chan;}
    void setRotateChannelX( int chan )    {_chRX = chan;}
    void setRotateChannelY( int chan )    {_chRY = chan;}
    void setRotateChannelZ( int chan )    {_chRZ = chan;}

    /** Set/get rotation scaling factor */
    void setRotationScale( osg::Vec3& rs ) { _rotScale = rs; }
    osg::Vec3 getRotationScale() const { return _rotScale; }

    void setButtonDevice( Button* bDevice ) {_buttonDevice = bDevice;}
    void setResetButton( int button ) {_resetButton = button;}

protected:      // methods
    ~AnalogTracker() {}

    AnalogTracker(const AnalogTracker&);
    const AnalogTracker& operator=(const AnalogTracker&);

protected:      // data
    osg::ref_ptr<Analog> _analogDevice;
    int _chTX, _chTY, _chTZ;
    int _chRX, _chRY, _chRZ;
    osg::Vec3 _rotScale;

    osg::ref_ptr<Button> _buttonDevice;
    int _resetButton;
    bool _previousResetState;

    osg::Matrixd _matrix;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
