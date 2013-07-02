/****************************************************************************
** Detector meta object code from reading C++ file 'detector.hh'
**
** Created: Tue Dec 25 06:30:04 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "detector.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Detector::className() const
{
    return "Detector";
}

QMetaObject *Detector::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Detector( "Detector", &Detector::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Detector::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Detector", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Detector::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Detector", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Detector::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"clearDetector", 0, 0 };
    static const QUMethod slot_1 = {"loadDetector", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "sDetName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_2 = {"loadDetector", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "sDetName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"loadDetector", 2, param_slot_3 };
    static const QUMethod slot_4 = {"saveDetector", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ "sDetName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_5 = {"saveDetector", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "sDetName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"saveDetector", 2, param_slot_6 };
    static const QUMethod slot_7 = {"addCue", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ "sCBName", &static_QUType_ptr, "string", QUParameter::In },
	{ "sOccName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_8 = {"addCue", 2, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "sCBName", &static_QUType_ptr, "string", QUParameter::In },
	{ "sOccName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"addCue", 3, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod slot_10 = {"loadCodebook", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"loadCodebook", 2, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_12 = {"loadCodebook", 2, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bLoadCB", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"loadCodebook", 3, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bLoadCB", &static_QUType_bool, 0, QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"loadCodebook", 4, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod slot_15 = {"loadOccurrences", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"loadOccurrences", 2, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_17 = {"loadOccurrences", 2, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"loadOccurrences", 3, param_slot_18 };
    static const QUMethod slot_19 = {"propagateDetectorUpdate", 0, 0 };
    static const QUParameter param_slot_20[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"propagateDetectorUpdate", 1, param_slot_20 };
    static const QUMethod slot_21 = {"propagateCueUpdate", 0, 0 };
    static const QUParameter param_slot_22[] = {
	{ "vvFeatures", &static_QUType_ptr, "vector<vector<FeatureVector>>", QUParameter::InOut },
	{ "vvFeaturesLeft", &static_QUType_ptr, "vector<vector<FeatureVector>>", QUParameter::InOut }
    };
    static const QUMethod slot_22 = {"compareFeatures", 2, param_slot_22 };
    static const QUParameter param_slot_23[] = {
	{ "vvFeatures", &static_QUType_ptr, "vector<vector<FeatureVector>>", QUParameter::InOut },
	{ "vvFeaturesLeft", &static_QUType_ptr, "vector<vector<FeatureVector>>", QUParameter::InOut },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_23 = {"compareFeatures", 3, param_slot_23 };
    static const QUParameter param_slot_24[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "vFeatures", &static_QUType_ptr, "vector<FeatureVector>", QUParameter::InOut }
    };
    static const QUMethod slot_24 = {"compareFeatures", 2, param_slot_24 };
    static const QUParameter param_slot_25[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "vFeatures", &static_QUType_ptr, "vector<FeatureVector>", QUParameter::InOut },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_25 = {"compareFeatures", 3, param_slot_25 };
    static const QUParameter param_slot_26[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "vFeatures", &static_QUType_ptr, "vector<FeatureVector>", QUParameter::InOut }
    };
    static const QUMethod slot_26 = {"compareFeaturesLeft", 2, param_slot_26 };
    static const QUParameter param_slot_27[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "vFeatures", &static_QUType_ptr, "vector<FeatureVector>", QUParameter::InOut },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_27 = {"compareFeaturesLeft", 3, param_slot_27 };
    static const QUParameter param_slot_28[] = {
	{ "nImgWidth", &static_QUType_int, 0, QUParameter::In },
	{ "nImgHeight", &static_QUType_int, 0, QUParameter::In },
	{ "nVotingBinSize", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vSegmentations", &static_QUType_ptr, "vector<Segmentation>", QUParameter::InOut },
	{ "bDisplayVS", &static_QUType_bool, 0, QUParameter::In },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_28 = {"processTestImg", 7, param_slot_28 };
    static const QUParameter param_slot_29[] = {
	{ "nImgWidth", &static_QUType_int, 0, QUParameter::In },
	{ "nImgHeight", &static_QUType_int, 0, QUParameter::In },
	{ "nVotingBinSize", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vSegmentations", &static_QUType_ptr, "vector<Segmentation>", QUParameter::InOut },
	{ "bDisplayVS", &static_QUType_bool, 0, QUParameter::In },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_29 = {"processTestImg", 8, param_slot_29 };
    static const QUParameter param_slot_30[] = {
	{ "nImgWidth", &static_QUType_int, 0, QUParameter::In },
	{ "nImgHeight", &static_QUType_int, 0, QUParameter::In },
	{ "nVotingBinSize", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vSegmentations", &static_QUType_ptr, "vector<Segmentation>", QUParameter::InOut },
	{ "bDisplayVS", &static_QUType_bool, 0, QUParameter::In },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In },
	{ "bPrintTiming", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_30 = {"processTestImg", 9, param_slot_30 };
    static const QUMethod slot_31 = {"getPatchHypotheses", 0, 0 };
    static const QUParameter param_slot_32[] = {
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_32 = {"getPatchHypotheses", 1, param_slot_32 };
    static const QUParameter param_slot_33[] = {
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_33 = {"getPatchHypotheses", 2, param_slot_33 };
    static const QUParameter param_slot_34[] = {
	{ "vHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In }
    };
    static const QUMethod slot_34 = {"adjustHypoScoresGP", 1, param_slot_34 };
    static const QUParameter param_slot_35[] = {
	{ "vHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In }
    };
    static const QUMethod slot_35 = {"removeDuplicateHypos", 1, param_slot_35 };
    static const QUParameter param_slot_36[] = {
	{ "vHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_36 = {"removeDuplicateHypos", 2, param_slot_36 };
    static const QUParameter param_slot_37[] = {
	{ "vHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "nObjWidth", &static_QUType_int, 0, QUParameter::In },
	{ "nObjHeight", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_37 = {"setHypoBBox", 3, param_slot_37 };
    static const QUParameter param_slot_38[] = {
	{ "hypo", &static_QUType_ptr, "Hypothesis", QUParameter::In },
	{ "nImgWidth", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_38 = {"flipHypothesis", 2, param_slot_38 };
    static const QUParameter param_slot_39[] = {
	{ "hypo", &static_QUType_ptr, "Hypothesis", QUParameter::In },
	{ "nObjWidth", &static_QUType_int, 0, QUParameter::In },
	{ "nObjHeight", &static_QUType_int, 0, QUParameter::In },
	{ "nImgWidth", &static_QUType_int, 0, QUParameter::In },
	{ "nImgHeight", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_39 = {"getHypoSegmentation", 5, param_slot_39 };
    static const QUParameter param_slot_40[] = {
	{ "hypo", &static_QUType_ptr, "Hypothesis", QUParameter::In },
	{ "nObjWidth", &static_QUType_int, 0, QUParameter::In },
	{ "nObjHeight", &static_QUType_int, 0, QUParameter::In },
	{ "nImgWidth", &static_QUType_int, 0, QUParameter::In },
	{ "nImgHeight", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_40 = {"getHypoSegmentationLeft", 5, param_slot_40 };
    static const QUParameter param_slot_41[] = {
	{ "vVotes", &static_QUType_ptr, "list<HoughVote>", QUParameter::In }
    };
    static const QUMethod slot_41 = {"splitUpCueVotes", 1, param_slot_41 };
    static const QUParameter param_slot_42[] = {
	{ "page", &static_QUType_int, 0, QUParameter::In },
	{ "x", &static_QUType_int, 0, QUParameter::In },
	{ "y", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_42 = {"showSupportingPatches", 3, param_slot_42 };
    static const QUParameter param_slot_43[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "fvWindowPos", &static_QUType_ptr, "FeatureVector", QUParameter::In }
    };
    static const QUMethod slot_43 = {"showSupportingPatches", 2, param_slot_43 };
    static const QUParameter param_slot_44[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "fvWindowPos", &static_QUType_ptr, "FeatureVector", QUParameter::In },
	{ "bDrawMaps", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_44 = {"showSupportingPatches", 3, param_slot_44 };
    static const QUMethod slot_45 = {"displayOccScaleFootprint", 0, 0 };
    static const QUMethod slot_46 = {"saveFVMatlab", 0, 0 };
    static const QUParameter param_slot_47[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod slot_47 = {"saveFVMatlab", 1, param_slot_47 };
    static const QUMethod slot_48 = {"saveClustersMatlab", 0, 0 };
    static const QUParameter param_slot_49[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod slot_49 = {"saveClustersMatlab", 1, param_slot_49 };
    static const QUMethod slot_50 = {"savePatchActivationsMatlab", 0, 0 };
    static const QUParameter param_slot_51[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod slot_51 = {"savePatchActivationsMatlab", 1, param_slot_51 };
    static const QUMethod slot_52 = {"saveClusterImages", 0, 0 };
    static const QUParameter param_slot_53[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod slot_53 = {"saveClusterImages", 1, param_slot_53 };
    static const QMetaData slot_tbl[] = {
	{ "clearDetector()", &slot_0, QMetaData::Public },
	{ "loadDetector()", &slot_1, QMetaData::Public },
	{ "loadDetector(string)", &slot_2, QMetaData::Public },
	{ "loadDetector(string,bool)", &slot_3, QMetaData::Public },
	{ "saveDetector()", &slot_4, QMetaData::Public },
	{ "saveDetector(string)", &slot_5, QMetaData::Public },
	{ "saveDetector(string,bool)", &slot_6, QMetaData::Public },
	{ "addCue()", &slot_7, QMetaData::Public },
	{ "addCue(string,string)", &slot_8, QMetaData::Public },
	{ "addCue(string,string,bool)", &slot_9, QMetaData::Public },
	{ "loadCodebook(unsigned)", &slot_10, QMetaData::Public },
	{ "loadCodebook(unsigned,bool)", &slot_11, QMetaData::Public },
	{ "loadCodebook(unsigned,string)", &slot_12, QMetaData::Public },
	{ "loadCodebook(unsigned,string,bool)", &slot_13, QMetaData::Public },
	{ "loadCodebook(unsigned,string,bool,bool)", &slot_14, QMetaData::Public },
	{ "loadOccurrences(unsigned)", &slot_15, QMetaData::Public },
	{ "loadOccurrences(unsigned,bool)", &slot_16, QMetaData::Public },
	{ "loadOccurrences(unsigned,string)", &slot_17, QMetaData::Public },
	{ "loadOccurrences(unsigned,string,bool)", &slot_18, QMetaData::Public },
	{ "propagateDetectorUpdate()", &slot_19, QMetaData::Public },
	{ "propagateDetectorUpdate(const QString&)", &slot_20, QMetaData::Public },
	{ "propagateCueUpdate()", &slot_21, QMetaData::Public },
	{ "compareFeatures(vector<vector<FeatureVector>>&,vector<vector<FeatureVector>>&)", &slot_22, QMetaData::Public },
	{ "compareFeatures(vector<vector<FeatureVector>>&,vector<vector<FeatureVector>>&,bool)", &slot_23, QMetaData::Public },
	{ "compareFeatures(unsigned,vector<FeatureVector>&)", &slot_24, QMetaData::Public },
	{ "compareFeatures(unsigned,vector<FeatureVector>&,bool)", &slot_25, QMetaData::Public },
	{ "compareFeaturesLeft(unsigned,vector<FeatureVector>&)", &slot_26, QMetaData::Public },
	{ "compareFeaturesLeft(unsigned,vector<FeatureVector>&,bool)", &slot_27, QMetaData::Public },
	{ "processTestImg(int,int,int,vector<Hypothesis>&,vector<Segmentation>&,bool,bool)", &slot_28, QMetaData::Public },
	{ "processTestImg(int,int,int,vector<Hypothesis>&,vector<Segmentation>&,bool,bool,bool)", &slot_29, QMetaData::Public },
	{ "processTestImg(int,int,int,vector<Hypothesis>&,vector<Segmentation>&,bool,bool,bool,bool)", &slot_30, QMetaData::Public },
	{ "getPatchHypotheses()", &slot_31, QMetaData::Public },
	{ "getPatchHypotheses(bool)", &slot_32, QMetaData::Public },
	{ "getPatchHypotheses(bool,bool)", &slot_33, QMetaData::Public },
	{ "adjustHypoScoresGP(const vector<Hypothesis>&)", &slot_34, QMetaData::Public },
	{ "removeDuplicateHypos(const vector<Hypothesis>&)", &slot_35, QMetaData::Public },
	{ "removeDuplicateHypos(const vector<Hypothesis>&,bool)", &slot_36, QMetaData::Public },
	{ "setHypoBBox(const vector<Hypothesis>&,int,int)", &slot_37, QMetaData::Public },
	{ "flipHypothesis(const Hypothesis&,int)", &slot_38, QMetaData::Public },
	{ "getHypoSegmentation(Hypothesis,int,int,int,int)", &slot_39, QMetaData::Public },
	{ "getHypoSegmentationLeft(Hypothesis,int,int,int,int)", &slot_40, QMetaData::Public },
	{ "splitUpCueVotes(const list<HoughVote>&)", &slot_41, QMetaData::Public },
	{ "showSupportingPatches(int,int,int)", &slot_42, QMetaData::Public },
	{ "showSupportingPatches(unsigned,FeatureVector)", &slot_43, QMetaData::Public },
	{ "showSupportingPatches(unsigned,FeatureVector,bool)", &slot_44, QMetaData::Public },
	{ "displayOccScaleFootprint()", &slot_45, QMetaData::Public },
	{ "saveFVMatlab()", &slot_46, QMetaData::Public },
	{ "saveFVMatlab(unsigned)", &slot_47, QMetaData::Public },
	{ "saveClustersMatlab()", &slot_48, QMetaData::Public },
	{ "saveClustersMatlab(unsigned)", &slot_49, QMetaData::Public },
	{ "savePatchActivationsMatlab()", &slot_50, QMetaData::Public },
	{ "savePatchActivationsMatlab(unsigned)", &slot_51, QMetaData::Public },
	{ "saveClusterImages()", &slot_52, QMetaData::Public },
	{ "saveClusterImages(unsigned)", &slot_53, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "Hypothesis", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0a", QUParameter::In },
	{ "bPermanent", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigDrawHypothesis", 3, param_signal_0 };
    static const QUMethod signal_1 = {"sigRefreshSrcImg", 0, 0 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod signal_2 = {"sigDisplayResultImg", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_varptr, "\x0f", QUParameter::In },
	{ 0, &static_QUType_ptr, "OpGrayImage", QUParameter::In }
    };
    static const QUMethod signal_3 = {"sigDisplayResultImg", 2, param_signal_3 };
    static const QUMethod signal_4 = {"sigDetectorChanged", 0, 0 };
    static const QUMethod signal_5 = {"sigCuesChanged", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "sigDrawHypothesis(const Hypothesis&,const QColor&,bool)", &signal_0, QMetaData::Public },
	{ "sigRefreshSrcImg()", &signal_1, QMetaData::Public },
	{ "sigDisplayResultImg(const QImage&)", &signal_2, QMetaData::Public },
	{ "sigDisplayResultImg(const QImage&,OpGrayImage)", &signal_3, QMetaData::Public },
	{ "sigDetectorChanged()", &signal_4, QMetaData::Public },
	{ "sigCuesChanged()", &signal_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Detector", parentObject,
	slot_tbl, 54,
	signal_tbl, 6,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Detector.setMetaObject( metaObj );
    return metaObj;
}

void* Detector::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Detector" ) )
	return this;
    return QObject::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL sigDrawHypothesis
void Detector::sigDrawHypothesis( const Hypothesis& t0, const QColor& t1, bool t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_varptr.set(o+2,&t1);
    static_QUType_bool.set(o+3,t2);
    activate_signal( clist, o );
}

// SIGNAL sigRefreshSrcImg
void Detector::sigRefreshSrcImg()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

// SIGNAL sigDisplayResultImg
void Detector::sigDisplayResultImg( const QImage& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL sigDisplayResultImg
void Detector::sigDisplayResultImg( const QImage& t0, OpGrayImage t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_varptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL sigDetectorChanged
void Detector::sigDetectorChanged()
{
    activate_signal( staticMetaObject()->signalOffset() + 4 );
}

// SIGNAL sigCuesChanged
void Detector::sigCuesChanged()
{
    activate_signal( staticMetaObject()->signalOffset() + 5 );
}

bool Detector::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: clearDetector(); break;
    case 1: loadDetector(); break;
    case 2: loadDetector((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 3: loadDetector((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 4: saveDetector(); break;
    case 5: saveDetector((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 6: saveDetector((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 7: addCue(); break;
    case 8: addCue((string)(*((string*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2)))); break;
    case 9: addCue((string)(*((string*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(bool)static_QUType_bool.get(_o+3)); break;
    case 10: loadCodebook((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1)))); break;
    case 11: loadCodebook((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 12: loadCodebook((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2)))); break;
    case 13: loadCodebook((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(bool)static_QUType_bool.get(_o+3)); break;
    case 14: loadCodebook((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(bool)static_QUType_bool.get(_o+3),(bool)static_QUType_bool.get(_o+4)); break;
    case 15: loadOccurrences((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1)))); break;
    case 16: loadOccurrences((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 17: loadOccurrences((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2)))); break;
    case 18: loadOccurrences((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(bool)static_QUType_bool.get(_o+3)); break;
    case 19: propagateDetectorUpdate(); break;
    case 20: propagateDetectorUpdate((const QString&)static_QUType_QString.get(_o+1)); break;
    case 21: propagateCueUpdate(); break;
    case 22: compareFeatures((vector<vector<FeatureVector> >&)*((vector<vector<FeatureVector> >*)static_QUType_ptr.get(_o+1)),(vector<vector<FeatureVector> >&)*((vector<vector<FeatureVector> >*)static_QUType_ptr.get(_o+2))); break;
    case 23: compareFeatures((vector<vector<FeatureVector> >&)*((vector<vector<FeatureVector> >*)static_QUType_ptr.get(_o+1)),(vector<vector<FeatureVector> >&)*((vector<vector<FeatureVector> >*)static_QUType_ptr.get(_o+2)),(bool)static_QUType_bool.get(_o+3)); break;
    case 24: compareFeatures((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(vector<FeatureVector>&)*((vector<FeatureVector>*)static_QUType_ptr.get(_o+2))); break;
    case 25: compareFeatures((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(vector<FeatureVector>&)*((vector<FeatureVector>*)static_QUType_ptr.get(_o+2)),(bool)static_QUType_bool.get(_o+3)); break;
    case 26: compareFeaturesLeft((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(vector<FeatureVector>&)*((vector<FeatureVector>*)static_QUType_ptr.get(_o+2))); break;
    case 27: compareFeaturesLeft((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(vector<FeatureVector>&)*((vector<FeatureVector>*)static_QUType_ptr.get(_o+2)),(bool)static_QUType_bool.get(_o+3)); break;
    case 28: processTestImg((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+4)),(vector<Segmentation>&)*((vector<Segmentation>*)static_QUType_ptr.get(_o+5)),(bool)static_QUType_bool.get(_o+6),(bool)static_QUType_bool.get(_o+7)); break;
    case 29: processTestImg((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+4)),(vector<Segmentation>&)*((vector<Segmentation>*)static_QUType_ptr.get(_o+5)),(bool)static_QUType_bool.get(_o+6),(bool)static_QUType_bool.get(_o+7),(bool)static_QUType_bool.get(_o+8)); break;
    case 30: processTestImg((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+4)),(vector<Segmentation>&)*((vector<Segmentation>*)static_QUType_ptr.get(_o+5)),(bool)static_QUType_bool.get(_o+6),(bool)static_QUType_bool.get(_o+7),(bool)static_QUType_bool.get(_o+8),(bool)static_QUType_bool.get(_o+9)); break;
    case 31: getPatchHypotheses(); break;
    case 32: getPatchHypotheses((bool)static_QUType_bool.get(_o+1)); break;
    case 33: getPatchHypotheses((bool)static_QUType_bool.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    case 34: adjustHypoScoresGP((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1))); break;
    case 35: removeDuplicateHypos((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1))); break;
    case 36: removeDuplicateHypos((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(bool)static_QUType_bool.get(_o+2)); break;
    case 37: setHypoBBox((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3)); break;
    case 38: flipHypothesis((const Hypothesis&)*((const Hypothesis*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2)); break;
    case 39: getHypoSegmentation((Hypothesis)(*((Hypothesis*)static_QUType_ptr.get(_o+1))),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(int)static_QUType_int.get(_o+4),(int)static_QUType_int.get(_o+5)); break;
    case 40: getHypoSegmentationLeft((Hypothesis)(*((Hypothesis*)static_QUType_ptr.get(_o+1))),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(int)static_QUType_int.get(_o+4),(int)static_QUType_int.get(_o+5)); break;
    case 41: splitUpCueVotes((const list<HoughVote>&)*((const list<HoughVote>*)static_QUType_ptr.get(_o+1))); break;
    case 42: showSupportingPatches((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3)); break;
    case 43: showSupportingPatches((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(FeatureVector)(*((FeatureVector*)static_QUType_ptr.get(_o+2)))); break;
    case 44: showSupportingPatches((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(FeatureVector)(*((FeatureVector*)static_QUType_ptr.get(_o+2))),(bool)static_QUType_bool.get(_o+3)); break;
    case 45: displayOccScaleFootprint(); break;
    case 46: saveFVMatlab(); break;
    case 47: saveFVMatlab((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1)))); break;
    case 48: saveClustersMatlab(); break;
    case 49: saveClustersMatlab((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1)))); break;
    case 50: savePatchActivationsMatlab(); break;
    case 51: savePatchActivationsMatlab((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1)))); break;
    case 52: saveClusterImages(); break;
    case 53: saveClusterImages((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Detector::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigDrawHypothesis((const Hypothesis&)*((const Hypothesis*)static_QUType_ptr.get(_o+1)),(const QColor&)*((const QColor*)static_QUType_ptr.get(_o+2)),(bool)static_QUType_bool.get(_o+3)); break;
    case 1: sigRefreshSrcImg(); break;
    case 2: sigDisplayResultImg((const QImage&)*((const QImage*)static_QUType_ptr.get(_o+1))); break;
    case 3: sigDisplayResultImg((const QImage&)*((const QImage*)static_QUType_ptr.get(_o+1)),(OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+2)))); break;
    case 4: sigDetectorChanged(); break;
    case 5: sigCuesChanged(); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool Detector::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool Detector::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
