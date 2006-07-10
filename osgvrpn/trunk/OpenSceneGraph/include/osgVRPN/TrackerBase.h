/* file:        include/osgVRPN/TrackerBase.h
 * author:      Mike Weiblen
 * copyright:   (C) 2006 Michael Weiblen http://mew.cx/
 * license:     OpenSceneGraph Public License (OSGPL)
 * $Id: TrackerBase.h,v 1.3 2006/07/10 06:16:27 mew Exp $
*/

#ifndef OSGVRPN_TRACKERBASE
#define OSGVRPN_TRACKERBASE 1

#include <osg/Vec3>
#include <osg/Matrixd>
#include <osg/Referenced>

#include <osgVRPN/Export>

namespace osgVRPN {

/** a base class for things that look like osgVRPN::Trackers */

class OSGVRPN_EXPORT TrackerBase : public osg::Referenced
{
public:
    TrackerBase() : _enabled(true), _eventCounter(0), _transScale(1,1,1) {}

    /** Set/get update enable flag */
    void setEnable( bool enabled ) {_enabled = enabled;}
    bool getEnable() const {return _enabled;}

    /** Update our state from the device. */
    virtual void update() = 0;

    void clearEventCounter() {_eventCounter = 0;}
    unsigned int getEventCounter() const {return _eventCounter;}

    /** Get the tracker's transform matrix */
    virtual osg::Matrixd getMatrix() const = 0;
    virtual osg::Matrixd getInverseMatrix() const = 0;

    /** Set/get translation scaling factor */
    void setTranslationScale( osg::Vec3& ts ) { _transScale = ts; }
    osg::Vec3 getTranslationScale() const { return _transScale; }

protected:      // methods
    virtual ~TrackerBase() {}

    TrackerBase(const TrackerBase&);
    const TrackerBase& operator=(const TrackerBase&);

protected:      // data
    bool _enabled;
    unsigned int _eventCounter;
    osg::Vec3 _transScale;
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:
