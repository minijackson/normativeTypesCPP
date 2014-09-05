/* ntscalarArray.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTSCALARARRAY_H
#define NTSCALARARRAY_H

#include <pv/ntfield.h>
#include <pv/pvDisplay.h>
#include <pv/pvControl.h>

namespace epics { namespace nt {

class NTScalarArray;
typedef std::tr1::shared_ptr<NTScalarArray> NTScalarArrayPtr;

namespace detail {

    /**
     * Interface for in-line creating of NTScalarArray.
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author mse
     */
    class epicsShareClass NTScalarArrayBuilder :
        public std::tr1::enable_shared_from_this<NTScalarArrayBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTScalarArrayBuilder);

        /**
         * Set a value type of a NTScalarArray.
         * @param elementType the value array element type.
         * @return this instance of a {@code NTTableBuilder}.
         */
        shared_pointer arrayValue(epics::pvData::ScalarType elementType);

        /**
         * Add descriptor field to the NTScalarArray.
         * @return this instance of a {@code NTScalarArrayBuilder}.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTScalarArray.
         * @return this instance of a {@code NTScalarArrayBuilder}.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTScalarArray.
         * @return this instance of a {@code NTScalarArrayBuilder}.
         */
        shared_pointer addTimeStamp();

        /**
         * Add display structure to the NTScalarArray.
         * @return this instance of a {@code NTScalarArrayBuilder}.
         */
        shared_pointer addDisplay();

        /**
         * Add control structure to the NTScalarArray.
         * @return this instance of a {@code NTScalarArrayBuilder}.
         */
        shared_pointer addControl();

        /**
         * Create a {@code Structure} that represents NTScalarArray.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a {@code Structure}.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a {@code PVStructure} that represents NTScalarArray.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code PVStructure}
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a {@code NTScalarArray} instance.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code NTScalarArray}
         */
        NTScalarArrayPtr create();

    private:
        NTScalarArrayBuilder();

        void reset();

        bool valueTypeSet;
        epics::pvData::ScalarType valueType;

        bool descriptor;
        bool alarm;
        bool timeStamp;
        bool display;
        bool control;

        friend class ::epics::nt::NTScalarArray;
    };

}

typedef std::tr1::shared_ptr<detail::NTScalarArrayBuilder> NTScalarArrayBuilderPtr;



/**
 * Convenience Class for NTScalarArray
 * @author mrk
 */
class NTScalarArray
{
public:
    POINTER_DEFINITIONS(NTScalarArray);

    static const std::string URI;

    /**
     * Narrow (aka dynamic cast, or wrap) the structure to NTScalarArray.
     * First the structure ID is checked against NTScalarArray::URI.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to narrow-ed (dynamic cast, wrapped) to NTScalarArray.
     * @return NTScalarArray instance on success, nullptr otherwise.
     */
    static shared_pointer narrow(epics::pvData::PVStructurePtr const & structure);

    /**
     * Narrow (aka dynamic cast, or wrap) the structure to NTScalarArray without checking for null-ness or its ID.
     * @param structure The structure to narrow-ed (dynamic cast, wrapped) to NTScalarArray.
     * @return NTScalarArray instance.
     */
    static shared_pointer narrow_unsafe(epics::pvData::PVStructurePtr const & structure);


    /**
     * Is the structure an NTScalarArray.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTScalarArray.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Create a NTScalarArray builder instance.
     * @return builder instance.
     */
    static NTScalarArrayBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTScalarArray() {}

     /**
      * Attach a pvTimeStamp.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
      */
    bool attachTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;

    /**
     * Attach an pvAlarm.
     * @param pvAlarm The pvAlarm that will be attached.
     * Does nothing if no alarm.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
     */
    bool attachAlarm(epics::pvData::PVAlarm &pvAlarm) const;

    /**
     * Attach an pvDisplay.
     * @param pvDisplay The pvDisplay that will be attached.
     * Does nothing if no display.
      * @return true if the operation was successfull (i.e. this instance has a display field), otherwise false.
     */
    bool attachDisplay(epics::pvData::PVDisplay &pvDisplay) const;

    /**
     * Attach an pvControl.
     * @param pvControl The pvControl that will be attached.
     * Does nothing if no control.
      * @return true if the operation was successfull (i.e. this instance has a control field), otherwise false.
     */
    bool attachControl(epics::pvData::PVControl &pvControl) const;

    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getPVStructure() const;

    /**
     * Get the descriptor field.
     * @return The pvString or null if no function field.
     */
    epics::pvData::PVStringPtr getDescriptor() const;

    /**
     * Get the timeStamp.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getTimeStamp() const;

    /**
     * Get the alarm.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getAlarm() const;

    /**
     * Get the display.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getDisplay() const;

    /**
     * Get the control.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getControl() const;

    /**
     * Get the value field.
     * @return The PVField for the values.
     */
    epics::pvData::PVFieldPtr getValue() const;

    /**
     * Get the value field of a specified type (e.g. PVDoubleArray).
     * @return The <PVT> field for the values.
     */
    template<typename PVT>
    std::tr1::shared_ptr<PVT> getValue() const
    {
        return std::tr1::dynamic_pointer_cast<PVT>(pvValue);
    }

private:
    NTScalarArray(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTScalarArray;
    epics::pvData::PVFieldPtr pvValue;
    friend class detail::NTScalarArrayBuilder;
};

}}
#endif  /* NTScalarArray_H */