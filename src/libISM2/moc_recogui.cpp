/****************************************************************************
** RecoGUI meta object code from reading C++ file 'recogui.hh'
**
** Created: Tue Jan 15 17:11:09 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "recogui.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RecoGUI::className() const
{
    return "RecoGUI";
}

QMetaObject *RecoGUI::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RecoGUI( "RecoGUI", &RecoGUI::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RecoGUI::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RecoGUI", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RecoGUI::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RecoGUI", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RecoGUI::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotSetMSMEx", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotSetMSMEy", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"slotSetMSMEs", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"slotSetMSMEr", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotSetMSMEa", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"slotSetMSMEp", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"slotSetAdaptMinScale", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"slotSetScoreThreshSingle", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"slotSetObjWidth", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"slotSetObjHeight", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"slotSetSearchRange", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"slotSetScaleMin", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"slotSetScaleMax", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"slotSetMinVoteWeight", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"slotSetMaxVoteWeight", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"slotSetMaxOverlap", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"slotSetMinPFig", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"slotSetWeightPFig", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"slotSetMinPFigRefined", 1, param_slot_18 };
    static const QUParameter param_slot_19[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_19 = {"slotSetAdaptMinMDLScale", 1, param_slot_19 };
    static const QUParameter param_slot_20[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"slotSetRotMin", 1, param_slot_20 };
    static const QUParameter param_slot_21[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_21 = {"slotSetRotMax", 1, param_slot_21 };
    static const QUParameter param_slot_22[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_22 = {"slotSetAspMin", 1, param_slot_22 };
    static const QUParameter param_slot_23[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_23 = {"slotSetAspMax", 1, param_slot_23 };
    static const QUParameter param_slot_24[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_24 = {"slotSetGibbsConst", 1, param_slot_24 };
    static const QUMethod slot_25 = {"slotUpdateMSMEx", 0, 0 };
    static const QUMethod slot_26 = {"slotUpdateMSMEy", 0, 0 };
    static const QUMethod slot_27 = {"slotUpdateMSMEs", 0, 0 };
    static const QUMethod slot_28 = {"slotUpdateMSMEr", 0, 0 };
    static const QUMethod slot_29 = {"slotUpdateMSMEa", 0, 0 };
    static const QUMethod slot_30 = {"slotUpdateMSMEp", 0, 0 };
    static const QUMethod slot_31 = {"slotUpdateAdaptMinScale", 0, 0 };
    static const QUMethod slot_32 = {"slotUpdateScoreThreshSingle", 0, 0 };
    static const QUMethod slot_33 = {"slotUpdateObjWidth", 0, 0 };
    static const QUMethod slot_34 = {"slotUpdateObjHeight", 0, 0 };
    static const QUMethod slot_35 = {"slotUpdateSearchRange", 0, 0 };
    static const QUMethod slot_36 = {"slotUpdateScaleMin", 0, 0 };
    static const QUMethod slot_37 = {"slotUpdateScaleMax", 0, 0 };
    static const QUMethod slot_38 = {"slotUpdateMinVoteWeight", 0, 0 };
    static const QUMethod slot_39 = {"slotUpdateMaxVoteWeight", 0, 0 };
    static const QUMethod slot_40 = {"slotUpdateMaxOverlap", 0, 0 };
    static const QUMethod slot_41 = {"slotUpdateMinPFig", 0, 0 };
    static const QUMethod slot_42 = {"slotUpdateWeightPFig", 0, 0 };
    static const QUMethod slot_43 = {"slotUpdateMinPFigRefined", 0, 0 };
    static const QUMethod slot_44 = {"slotUpdateAdaptMinMDLScale", 0, 0 };
    static const QUMethod slot_45 = {"slotUpdateRotMin", 0, 0 };
    static const QUMethod slot_46 = {"slotUpdateRotMax", 0, 0 };
    static const QUMethod slot_47 = {"slotUpdateAspMin", 0, 0 };
    static const QUMethod slot_48 = {"slotUpdateAspMax", 0, 0 };
    static const QUMethod slot_49 = {"slotUpdateGibbsConst", 0, 0 };
    static const QUParameter param_slot_50[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_50 = {"slotSelectKernelType", 1, param_slot_50 };
    static const QUParameter param_slot_51[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_51 = {"slotSelectFixObjDim", 1, param_slot_51 };
    static const QUParameter param_slot_52[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_52 = {"slotSelectMatchWeighting", 1, param_slot_52 };
    static const QUParameter param_slot_53[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_53 = {"slotSetAdaptiveScaleOnOff", 1, param_slot_53 };
    static const QUParameter param_slot_54[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_54 = {"slotSetNormScalePFig2OnOff", 1, param_slot_54 };
    static const QUParameter param_slot_55[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_55 = {"slotSetRestrictScaleOnOff", 1, param_slot_55 };
    static const QUParameter param_slot_56[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_56 = {"slotSetUseFastMSMEOnOff", 1, param_slot_56 };
    static const QUParameter param_slot_57[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_57 = {"slotSetExtendSearchOnOff", 1, param_slot_57 };
    static const QUParameter param_slot_58[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_58 = {"slotSetNormPatchOnOff", 1, param_slot_58 };
    static const QUParameter param_slot_59[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_59 = {"slotSetNormPoseOnOff", 1, param_slot_59 };
    static const QUParameter param_slot_60[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_60 = {"slotSetDoMDLOnOff", 1, param_slot_60 };
    static const QUParameter param_slot_61[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_61 = {"slotSetRejectOverlapOnOff", 1, param_slot_61 };
    static const QUParameter param_slot_62[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_62 = {"slotSetRejectPFigOnOff", 1, param_slot_62 };
    static const QUParameter param_slot_63[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_63 = {"slotSetResampleHyposOnOff", 1, param_slot_63 };
    static const QUParameter param_slot_64[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_64 = {"slotSetRecoRotInvOnOff", 1, param_slot_64 };
    static const QUParameter param_slot_65[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_65 = {"slotSetRecoverRotOnOff", 1, param_slot_65 };
    static const QUParameter param_slot_66[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_66 = {"slotSetUseAspectOnOff", 1, param_slot_66 };
    static const QMetaData slot_tbl[] = {
	{ "slotSetMSMEx(const QString&)", &slot_0, QMetaData::Public },
	{ "slotSetMSMEy(const QString&)", &slot_1, QMetaData::Public },
	{ "slotSetMSMEs(const QString&)", &slot_2, QMetaData::Public },
	{ "slotSetMSMEr(const QString&)", &slot_3, QMetaData::Public },
	{ "slotSetMSMEa(const QString&)", &slot_4, QMetaData::Public },
	{ "slotSetMSMEp(const QString&)", &slot_5, QMetaData::Public },
	{ "slotSetAdaptMinScale(const QString&)", &slot_6, QMetaData::Public },
	{ "slotSetScoreThreshSingle(const QString&)", &slot_7, QMetaData::Public },
	{ "slotSetObjWidth(const QString&)", &slot_8, QMetaData::Public },
	{ "slotSetObjHeight(const QString&)", &slot_9, QMetaData::Public },
	{ "slotSetSearchRange(const QString&)", &slot_10, QMetaData::Public },
	{ "slotSetScaleMin(const QString&)", &slot_11, QMetaData::Public },
	{ "slotSetScaleMax(const QString&)", &slot_12, QMetaData::Public },
	{ "slotSetMinVoteWeight(const QString&)", &slot_13, QMetaData::Public },
	{ "slotSetMaxVoteWeight(const QString&)", &slot_14, QMetaData::Public },
	{ "slotSetMaxOverlap(const QString&)", &slot_15, QMetaData::Public },
	{ "slotSetMinPFig(const QString&)", &slot_16, QMetaData::Public },
	{ "slotSetWeightPFig(const QString&)", &slot_17, QMetaData::Public },
	{ "slotSetMinPFigRefined(const QString&)", &slot_18, QMetaData::Public },
	{ "slotSetAdaptMinMDLScale(const QString&)", &slot_19, QMetaData::Public },
	{ "slotSetRotMin(const QString&)", &slot_20, QMetaData::Public },
	{ "slotSetRotMax(const QString&)", &slot_21, QMetaData::Public },
	{ "slotSetAspMin(const QString&)", &slot_22, QMetaData::Public },
	{ "slotSetAspMax(const QString&)", &slot_23, QMetaData::Public },
	{ "slotSetGibbsConst(const QString&)", &slot_24, QMetaData::Public },
	{ "slotUpdateMSMEx()", &slot_25, QMetaData::Public },
	{ "slotUpdateMSMEy()", &slot_26, QMetaData::Public },
	{ "slotUpdateMSMEs()", &slot_27, QMetaData::Public },
	{ "slotUpdateMSMEr()", &slot_28, QMetaData::Public },
	{ "slotUpdateMSMEa()", &slot_29, QMetaData::Public },
	{ "slotUpdateMSMEp()", &slot_30, QMetaData::Public },
	{ "slotUpdateAdaptMinScale()", &slot_31, QMetaData::Public },
	{ "slotUpdateScoreThreshSingle()", &slot_32, QMetaData::Public },
	{ "slotUpdateObjWidth()", &slot_33, QMetaData::Public },
	{ "slotUpdateObjHeight()", &slot_34, QMetaData::Public },
	{ "slotUpdateSearchRange()", &slot_35, QMetaData::Public },
	{ "slotUpdateScaleMin()", &slot_36, QMetaData::Public },
	{ "slotUpdateScaleMax()", &slot_37, QMetaData::Public },
	{ "slotUpdateMinVoteWeight()", &slot_38, QMetaData::Public },
	{ "slotUpdateMaxVoteWeight()", &slot_39, QMetaData::Public },
	{ "slotUpdateMaxOverlap()", &slot_40, QMetaData::Public },
	{ "slotUpdateMinPFig()", &slot_41, QMetaData::Public },
	{ "slotUpdateWeightPFig()", &slot_42, QMetaData::Public },
	{ "slotUpdateMinPFigRefined()", &slot_43, QMetaData::Public },
	{ "slotUpdateAdaptMinMDLScale()", &slot_44, QMetaData::Public },
	{ "slotUpdateRotMin()", &slot_45, QMetaData::Public },
	{ "slotUpdateRotMax()", &slot_46, QMetaData::Public },
	{ "slotUpdateAspMin()", &slot_47, QMetaData::Public },
	{ "slotUpdateAspMax()", &slot_48, QMetaData::Public },
	{ "slotUpdateGibbsConst()", &slot_49, QMetaData::Public },
	{ "slotSelectKernelType(int)", &slot_50, QMetaData::Public },
	{ "slotSelectFixObjDim(int)", &slot_51, QMetaData::Public },
	{ "slotSelectMatchWeighting(int)", &slot_52, QMetaData::Public },
	{ "slotSetAdaptiveScaleOnOff(int)", &slot_53, QMetaData::Public },
	{ "slotSetNormScalePFig2OnOff(int)", &slot_54, QMetaData::Public },
	{ "slotSetRestrictScaleOnOff(int)", &slot_55, QMetaData::Public },
	{ "slotSetUseFastMSMEOnOff(int)", &slot_56, QMetaData::Public },
	{ "slotSetExtendSearchOnOff(int)", &slot_57, QMetaData::Public },
	{ "slotSetNormPatchOnOff(int)", &slot_58, QMetaData::Public },
	{ "slotSetNormPoseOnOff(int)", &slot_59, QMetaData::Public },
	{ "slotSetDoMDLOnOff(int)", &slot_60, QMetaData::Public },
	{ "slotSetRejectOverlapOnOff(int)", &slot_61, QMetaData::Public },
	{ "slotSetRejectPFigOnOff(int)", &slot_62, QMetaData::Public },
	{ "slotSetResampleHyposOnOff(int)", &slot_63, QMetaData::Public },
	{ "slotSetRecoRotInvOnOff(int)", &slot_64, QMetaData::Public },
	{ "slotSetRecoverRotOnOff(int)", &slot_65, QMetaData::Public },
	{ "slotSetUseAspectOnOff(int)", &slot_66, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigMSMExChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"sigMSMEyChanged", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"sigMSMEsChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"sigMSMErChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"sigMSMEaChanged", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"sigMSMEpChanged", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_6 = {"sigAdaptMinScaleChanged", 1, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"sigScoreThreshSingleChanged", 1, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_8 = {"sigObjWidthChanged", 1, param_signal_8 };
    static const QUParameter param_signal_9[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_9 = {"sigObjHeightChanged", 1, param_signal_9 };
    static const QUParameter param_signal_10[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_10 = {"sigSearchRangeChanged", 1, param_signal_10 };
    static const QUParameter param_signal_11[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_11 = {"sigScaleMinChanged", 1, param_signal_11 };
    static const QUParameter param_signal_12[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_12 = {"sigScaleMaxChanged", 1, param_signal_12 };
    static const QUParameter param_signal_13[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_13 = {"sigMinVoteWeightChanged", 1, param_signal_13 };
    static const QUParameter param_signal_14[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_14 = {"sigMaxVoteWeightChanged", 1, param_signal_14 };
    static const QUParameter param_signal_15[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_15 = {"sigMaxOverlapChanged", 1, param_signal_15 };
    static const QUParameter param_signal_16[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_16 = {"sigMinPFigChanged", 1, param_signal_16 };
    static const QUParameter param_signal_17[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_17 = {"sigWeightPFigChanged", 1, param_signal_17 };
    static const QUParameter param_signal_18[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_18 = {"sigMinPFigRefinedChanged", 1, param_signal_18 };
    static const QUParameter param_signal_19[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_19 = {"sigAdaptMinMDLScaleChanged", 1, param_signal_19 };
    static const QUParameter param_signal_20[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_20 = {"sigRotMinChanged", 1, param_signal_20 };
    static const QUParameter param_signal_21[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_21 = {"sigRotMaxChanged", 1, param_signal_21 };
    static const QUParameter param_signal_22[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_22 = {"sigAspMinChanged", 1, param_signal_22 };
    static const QUParameter param_signal_23[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_23 = {"sigAspMaxChanged", 1, param_signal_23 };
    static const QUParameter param_signal_24[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_24 = {"sigGibbsConstChanged", 1, param_signal_24 };
    static const QMetaData signal_tbl[] = {
	{ "sigMSMExChanged(const QString&)", &signal_0, QMetaData::Public },
	{ "sigMSMEyChanged(const QString&)", &signal_1, QMetaData::Public },
	{ "sigMSMEsChanged(const QString&)", &signal_2, QMetaData::Public },
	{ "sigMSMErChanged(const QString&)", &signal_3, QMetaData::Public },
	{ "sigMSMEaChanged(const QString&)", &signal_4, QMetaData::Public },
	{ "sigMSMEpChanged(const QString&)", &signal_5, QMetaData::Public },
	{ "sigAdaptMinScaleChanged(const QString&)", &signal_6, QMetaData::Public },
	{ "sigScoreThreshSingleChanged(const QString&)", &signal_7, QMetaData::Public },
	{ "sigObjWidthChanged(const QString&)", &signal_8, QMetaData::Public },
	{ "sigObjHeightChanged(const QString&)", &signal_9, QMetaData::Public },
	{ "sigSearchRangeChanged(const QString&)", &signal_10, QMetaData::Public },
	{ "sigScaleMinChanged(const QString&)", &signal_11, QMetaData::Public },
	{ "sigScaleMaxChanged(const QString&)", &signal_12, QMetaData::Public },
	{ "sigMinVoteWeightChanged(const QString&)", &signal_13, QMetaData::Public },
	{ "sigMaxVoteWeightChanged(const QString&)", &signal_14, QMetaData::Public },
	{ "sigMaxOverlapChanged(const QString&)", &signal_15, QMetaData::Public },
	{ "sigMinPFigChanged(const QString&)", &signal_16, QMetaData::Public },
	{ "sigWeightPFigChanged(const QString&)", &signal_17, QMetaData::Public },
	{ "sigMinPFigRefinedChanged(const QString&)", &signal_18, QMetaData::Public },
	{ "sigAdaptMinMDLScaleChanged(const QString&)", &signal_19, QMetaData::Public },
	{ "sigRotMinChanged(const QString&)", &signal_20, QMetaData::Public },
	{ "sigRotMaxChanged(const QString&)", &signal_21, QMetaData::Public },
	{ "sigAspMinChanged(const QString&)", &signal_22, QMetaData::Public },
	{ "sigAspMaxChanged(const QString&)", &signal_23, QMetaData::Public },
	{ "sigGibbsConstChanged(const QString&)", &signal_24, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"RecoGUI", parentObject,
	slot_tbl, 67,
	signal_tbl, 25,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RecoGUI.setMetaObject( metaObj );
    return metaObj;
}

void* RecoGUI::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RecoGUI" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sigMSMExChanged
void RecoGUI::sigMSMExChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL sigMSMEyChanged
void RecoGUI::sigMSMEyChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL sigMSMEsChanged
void RecoGUI::sigMSMEsChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL sigMSMErChanged
void RecoGUI::sigMSMErChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL sigMSMEaChanged
void RecoGUI::sigMSMEaChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL sigMSMEpChanged
void RecoGUI::sigMSMEpChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL sigAdaptMinScaleChanged
void RecoGUI::sigAdaptMinScaleChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 6, t0 );
}

// SIGNAL sigScoreThreshSingleChanged
void RecoGUI::sigScoreThreshSingleChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 7, t0 );
}

// SIGNAL sigObjWidthChanged
void RecoGUI::sigObjWidthChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 8, t0 );
}

// SIGNAL sigObjHeightChanged
void RecoGUI::sigObjHeightChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 9, t0 );
}

// SIGNAL sigSearchRangeChanged
void RecoGUI::sigSearchRangeChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 10, t0 );
}

// SIGNAL sigScaleMinChanged
void RecoGUI::sigScaleMinChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 11, t0 );
}

// SIGNAL sigScaleMaxChanged
void RecoGUI::sigScaleMaxChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 12, t0 );
}

// SIGNAL sigMinVoteWeightChanged
void RecoGUI::sigMinVoteWeightChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 13, t0 );
}

// SIGNAL sigMaxVoteWeightChanged
void RecoGUI::sigMaxVoteWeightChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 14, t0 );
}

// SIGNAL sigMaxOverlapChanged
void RecoGUI::sigMaxOverlapChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 15, t0 );
}

// SIGNAL sigMinPFigChanged
void RecoGUI::sigMinPFigChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 16, t0 );
}

// SIGNAL sigWeightPFigChanged
void RecoGUI::sigWeightPFigChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 17, t0 );
}

// SIGNAL sigMinPFigRefinedChanged
void RecoGUI::sigMinPFigRefinedChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 18, t0 );
}

// SIGNAL sigAdaptMinMDLScaleChanged
void RecoGUI::sigAdaptMinMDLScaleChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 19, t0 );
}

// SIGNAL sigRotMinChanged
void RecoGUI::sigRotMinChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 20, t0 );
}

// SIGNAL sigRotMaxChanged
void RecoGUI::sigRotMaxChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 21, t0 );
}

// SIGNAL sigAspMinChanged
void RecoGUI::sigAspMinChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 22, t0 );
}

// SIGNAL sigAspMaxChanged
void RecoGUI::sigAspMaxChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 23, t0 );
}

// SIGNAL sigGibbsConstChanged
void RecoGUI::sigGibbsConstChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 24, t0 );
}

bool RecoGUI::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotSetMSMEx((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: slotSetMSMEy((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: slotSetMSMEs((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: slotSetMSMEr((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: slotSetMSMEa((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: slotSetMSMEp((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: slotSetAdaptMinScale((const QString&)static_QUType_QString.get(_o+1)); break;
    case 7: slotSetScoreThreshSingle((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: slotSetObjWidth((const QString&)static_QUType_QString.get(_o+1)); break;
    case 9: slotSetObjHeight((const QString&)static_QUType_QString.get(_o+1)); break;
    case 10: slotSetSearchRange((const QString&)static_QUType_QString.get(_o+1)); break;
    case 11: slotSetScaleMin((const QString&)static_QUType_QString.get(_o+1)); break;
    case 12: slotSetScaleMax((const QString&)static_QUType_QString.get(_o+1)); break;
    case 13: slotSetMinVoteWeight((const QString&)static_QUType_QString.get(_o+1)); break;
    case 14: slotSetMaxVoteWeight((const QString&)static_QUType_QString.get(_o+1)); break;
    case 15: slotSetMaxOverlap((const QString&)static_QUType_QString.get(_o+1)); break;
    case 16: slotSetMinPFig((const QString&)static_QUType_QString.get(_o+1)); break;
    case 17: slotSetWeightPFig((const QString&)static_QUType_QString.get(_o+1)); break;
    case 18: slotSetMinPFigRefined((const QString&)static_QUType_QString.get(_o+1)); break;
    case 19: slotSetAdaptMinMDLScale((const QString&)static_QUType_QString.get(_o+1)); break;
    case 20: slotSetRotMin((const QString&)static_QUType_QString.get(_o+1)); break;
    case 21: slotSetRotMax((const QString&)static_QUType_QString.get(_o+1)); break;
    case 22: slotSetAspMin((const QString&)static_QUType_QString.get(_o+1)); break;
    case 23: slotSetAspMax((const QString&)static_QUType_QString.get(_o+1)); break;
    case 24: slotSetGibbsConst((const QString&)static_QUType_QString.get(_o+1)); break;
    case 25: slotUpdateMSMEx(); break;
    case 26: slotUpdateMSMEy(); break;
    case 27: slotUpdateMSMEs(); break;
    case 28: slotUpdateMSMEr(); break;
    case 29: slotUpdateMSMEa(); break;
    case 30: slotUpdateMSMEp(); break;
    case 31: slotUpdateAdaptMinScale(); break;
    case 32: slotUpdateScoreThreshSingle(); break;
    case 33: slotUpdateObjWidth(); break;
    case 34: slotUpdateObjHeight(); break;
    case 35: slotUpdateSearchRange(); break;
    case 36: slotUpdateScaleMin(); break;
    case 37: slotUpdateScaleMax(); break;
    case 38: slotUpdateMinVoteWeight(); break;
    case 39: slotUpdateMaxVoteWeight(); break;
    case 40: slotUpdateMaxOverlap(); break;
    case 41: slotUpdateMinPFig(); break;
    case 42: slotUpdateWeightPFig(); break;
    case 43: slotUpdateMinPFigRefined(); break;
    case 44: slotUpdateAdaptMinMDLScale(); break;
    case 45: slotUpdateRotMin(); break;
    case 46: slotUpdateRotMax(); break;
    case 47: slotUpdateAspMin(); break;
    case 48: slotUpdateAspMax(); break;
    case 49: slotUpdateGibbsConst(); break;
    case 50: slotSelectKernelType((int)static_QUType_int.get(_o+1)); break;
    case 51: slotSelectFixObjDim((int)static_QUType_int.get(_o+1)); break;
    case 52: slotSelectMatchWeighting((int)static_QUType_int.get(_o+1)); break;
    case 53: slotSetAdaptiveScaleOnOff((int)static_QUType_int.get(_o+1)); break;
    case 54: slotSetNormScalePFig2OnOff((int)static_QUType_int.get(_o+1)); break;
    case 55: slotSetRestrictScaleOnOff((int)static_QUType_int.get(_o+1)); break;
    case 56: slotSetUseFastMSMEOnOff((int)static_QUType_int.get(_o+1)); break;
    case 57: slotSetExtendSearchOnOff((int)static_QUType_int.get(_o+1)); break;
    case 58: slotSetNormPatchOnOff((int)static_QUType_int.get(_o+1)); break;
    case 59: slotSetNormPoseOnOff((int)static_QUType_int.get(_o+1)); break;
    case 60: slotSetDoMDLOnOff((int)static_QUType_int.get(_o+1)); break;
    case 61: slotSetRejectOverlapOnOff((int)static_QUType_int.get(_o+1)); break;
    case 62: slotSetRejectPFigOnOff((int)static_QUType_int.get(_o+1)); break;
    case 63: slotSetResampleHyposOnOff((int)static_QUType_int.get(_o+1)); break;
    case 64: slotSetRecoRotInvOnOff((int)static_QUType_int.get(_o+1)); break;
    case 65: slotSetRecoverRotOnOff((int)static_QUType_int.get(_o+1)); break;
    case 66: slotSetUseAspectOnOff((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool RecoGUI::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigMSMExChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: sigMSMEyChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: sigMSMEsChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: sigMSMErChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: sigMSMEaChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: sigMSMEpChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: sigAdaptMinScaleChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 7: sigScoreThreshSingleChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: sigObjWidthChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 9: sigObjHeightChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 10: sigSearchRangeChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 11: sigScaleMinChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 12: sigScaleMaxChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 13: sigMinVoteWeightChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 14: sigMaxVoteWeightChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 15: sigMaxOverlapChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 16: sigMinPFigChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 17: sigWeightPFigChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 18: sigMinPFigRefinedChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 19: sigAdaptMinMDLScaleChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 20: sigRotMinChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 21: sigRotMaxChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 22: sigAspMinChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 23: sigAspMaxChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 24: sigGibbsConstChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool RecoGUI::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool RecoGUI::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
