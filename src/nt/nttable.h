/* nttable.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTTABLE_H
#define NTTABLE_H

#include <pv/ntfield.h>

namespace epics { namespace pvData { 

/**
 * Convenience Class for NTTable
 * @author mrk
 *
 */

class NTTable
typedef std::tr1::shared_ptr<NTTable> NTTablePtr;

class NTTable
{
public:
    POINTER_DEFINITIONS(NTTable);
    /**
     * Is the pvStructure an NTTable.
     * @param pvStructure The pvStructure to test.
     * @return (false,true) if (is not, is) an NTNameValuePair.
     */
    static bool isNTTable(PVStructurePtr const &pvStructure);
    /**
     * Create an  NTTable pvStructure.
     * @param hasFunction Create a PVString field named function.
     * @param hasTimeStamp Create a timeStamp structure field.
     * @param hasAlarm Create an alarm structure field.
     * @param numberValues The number of fields that follow the label field.
     * @param valueFields The fields that follow the label field.
     * @return an NTTable pvStructure.
     */
    static PVStructurePtr create(
        bool hasFunction,bool hasTimeStamp, bool hasAlarm,
        size_t numberValues,
        FieldConstPtrArray const &valueFields);
    /**
     * Constructor
     * @param pvStructure The pvStructure to which to attach.
     * @return A NTTable that is attached to the pvStructure
     */
    NTTable(PVStructurePtr const & pvStructure);
    /**
     * Destructor
     */
    ~NTTable();
    /**
     * Get the function field.
     * @return The pvString or null if no function field.
     */
    PVStringPtr getFunction();
     /**
      * Attach a pvTimeStamp.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp
      */
    void attachTimeStamp(PVTimeStamp &pvTimeStamp);
    /**
     * Attach an pvAlarm.
     * @param pvAlarm The pvAlarm that will be attached.
     * Does nothing if no alarm
     */
    void attachAlarm(PVAlarm &pvAlarm);
    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    PVStructurePtr getPVStructure(){return pvNTTable;}
    /**
     * Get the timeStamp.
     * @return PVStructurePtr which may be null.
     */
    PVStructurePtr getTimeStamp(){return pvTimeStamp;}
    /**
     * Get the alarm.
     * @return PVStructurePtr which may be null.
     */
    PVStructurePtr getAlarm() {return pvAlarm;}
    /**
     * Get the label field.
     * @return The pvStringArray for the label.
     */
    PVStringArrayPtr getLabel();
    /**
     * Get the the number of fields that follow the label field.
     * @return The number of fields.
     */
    int getNumberValues();
    /**
     * Get the Field for a field that follows the label field.
     * @param index The index of the field desired.
     * @return The FieldConstPtr for the field.
     */
    FieldConstPtr getField(int index);
    /**
     * Get the PVField for a field that follows the label field.
     * @param index The index of the field desired.
     * @return The PVFieldPtr for the field.
     */
    PVFieldPtr getPVField(int index);
private:
    PVStructurePtr pvNTTable;
    PVStringPtr pvFunction;
    PVStructurePtr pvTimeStamp;
    PVStructurePtr pvAlarm;
    PVStringArrayPtr pvLabel;
    size_t offsetFields;
};

}}
#endif  /* NTTABLE_H */
