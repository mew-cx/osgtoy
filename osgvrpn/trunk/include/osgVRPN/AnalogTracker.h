/* file:        include/osgVRPN/AnalogTracker.h
 * author:      Mike Weiblen
 * copyright:   (C) 2006-2011 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id$
*/

#ifndef OSGVRPN_ANALOGTRACKER
#define OSGVRPN_ANALOGTRACKER 1

#include <osg/ref_ptr>
#include <osg/Vec3>
#include <osg/Matrixd>
namespace osgViewer {
class Viewer;
}

#include <osgVRPN/TrackerBase.h>
#include <osgVRPN/Analog.h>
#include <osgVRPN/Button.h>

#include <osgVRPN/Export.h>

namespace osgVRPN {

/** a TrackerBase which integrates the output of several vrpn_Analog_Remote channels. */

class OSGVRPN_EXPORT AnalogTracker : public TrackerBase
{
public:
    AnalogTracker();

    /** Update our state from the device. */
    void update();

    void reset() { _matrix.makeIdentity(); }

    /** Query the tracker's transform matrix */
    const osg::Matrixd getMatrix() const {return _matrix;}
    const osg::Matrixd getInverseMatrix() const {return osg::Matrixd::inverse(_matrix);}

    void setAnalogDevice( Analog* aDevice ) {_analogDevice = aDevice;}
    void setTranslateChannelX( int chan ) {_chTX = chan;}
    void setTranslateChannelY( int chan ) {_chTY = chan;}
    void setTranslateChannelZ( int chan ) {_chTZ = chan;}
    void setRotateChannelX( int chan )    {_chRX = chan;}
    void setRotateChannelY( int chan )    {_chRY = chan;}
    void setRotateChannelZ( int chan )    {_chRZ = chan;}

    void setButtonDevice( Button* bDevice ) {_buttonDevice = bDevice;}
    void setResetButton( int button ) {_resetButton = button;}

    /** Set/get rotation scaling factor */
    void setRotationScale( const osg::Vec3& rs ) { _rotScale = rs; }
    const osg::Vec3& getRotationScale() const { return _rotScale; }

    /** Set the viewer that this Tracker will be relative to */
    void setViewer( osgViewer::Viewer* viewer ) { _viewer = viewer; }

protected:      // methods
    ~AnalogTracker() {}

protected:      // data
    osg::ref_ptr<Analog> _analogDevice;
    int _chTX, _chTY, _chTZ;
    int _chRX, _chRY, _chRZ;
    osg::Vec3 _rotScale;

    osg::ref_ptr<Button> _buttonDevice;
    int _resetButton;
    bool _previousResetButtonState;

    osg::Matrixd _matrix;

    osgViewer::Viewer* _viewer;

private:        // uncopyable
    AnalogTracker(const AnalogTracker&);
    AnalogTracker& operator=(const AnalogTracker&);
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
