/****************************************************************************
** ISMReco meta object code from reading C++ file 'scmatcher.hh'
**
** Created: Wed Jan 16 09:20:31 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "scmatcher.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#include <qvariant.h>
const char *ISMReco::className() const
{
    return "ISMReco";
}

QMetaObject *ISMReco::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ISMReco( "ISMReco", &ISMReco::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ISMReco::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ISMReco", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ISMReco::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ISMReco", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ISMReco::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotSetMapThresh", 1, param_slot_0 };
    static const QUMethod slot_1 = {"slotUpdateMapThresh", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"slotSetCateg", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"slotSetPose", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotSetUIUCFrom", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"slotSetObjWidth", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"slotSetObjHeight", 1, param_slot_6 };
    static const QUMethod slot_7 = {"slotUpdateCateg", 0, 0 };
    static const QUMethod slot_8 = {"slotUpdatePose", 0, 0 };
    static const QUMethod slot_9 = {"slotUpdateUIUCFrom", 0, 0 };
    static const QUMethod slot_10 = {"slotUpdateObjWidth", 0, 0 };
    static const QUMethod slot_11 = {"slotUpdateObjHeight", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"slotSelectFixObjDim", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"slotSetHistEqOnOff", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"slotSetUsePatchesOnOff", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"slotSetMapsOnOff", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"slotSetDrawConfOnOff", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"slotSetDrawMatchedPsOnOff", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"slotSetFigureOnOff", 1, param_slot_18 };
    static const QUParameter param_slot_19[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_19 = {"slotSetDrawTightBBOnOff", 1, param_slot_19 };
    static const QUParameter param_slot_20[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"slotSetDisplayVSOnOff", 1, param_slot_20 };
    static const QUParameter param_slot_21[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_21 = {"slotSetDisplaySupportOnOff", 1, param_slot_21 };
    static const QUParameter param_slot_22[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_22 = {"slotSetDisplaySegmentOnOff", 1, param_slot_22 };
    static const QUParameter param_slot_23[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_23 = {"slotSetSavePoseOnOff", 1, param_slot_23 };
    static const QUParameter param_slot_24[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_24 = {"slotSetRefineHypoOnOff", 1, param_slot_24 };
    static const QUParameter param_slot_25[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_25 = {"slotSetWriteDetectOnOff", 1, param_slot_25 };
    static const QUParameter param_slot_26[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_26 = {"slotSetDrawRejectedOnOff", 1, param_slot_26 };
    static const QUMethod slot_27 = {"loadImage", 0, 0 };
    static const QUParameter param_slot_28[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_28 = {"loadImage", 1, param_slot_28 };
    static const QUParameter param_slot_29[] = {
	{ "qsName", &static_QUType_QString, 0, QUParameter::In },
	{ "rBBox", &static_QUType_ptr, "Rect", QUParameter::In }
    };
    static const QUMethod slot_29 = {"loadImageBBox", 2, param_slot_29 };
    static const QUMethod slot_30 = {"loadCodebook", 0, 0 };
    static const QUParameter param_slot_31[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_31 = {"loadCodebook", 1, param_slot_31 };
    static const QUParameter param_slot_32[] = {
	{ "qstr", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_32 = {"processImage", 1, param_slot_32 };
    static const QUParameter param_slot_33[] = {
	{ "qstr", &static_QUType_QString, 0, QUParameter::In },
	{ "rBBox", &static_QUType_ptr, "Rect", QUParameter::In }
    };
    static const QUMethod slot_33 = {"processImageBBox", 2, param_slot_33 };
    static const QUMethod slot_34 = {"collectPatches", 0, 0 };
    static const QUParameter param_slot_35[] = {
	{ "process", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_35 = {"collectPatches", 1, param_slot_35 };
    static const QUMethod slot_36 = {"drawInterestPoints", 0, 0 };
    static const QUMethod slot_37 = {"drawInterestPointsEllipse", 0, 0 };
    static const QUParameter param_slot_38[] = {
	{ "vPatches", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut }
    };
    static const QUMethod slot_38 = {"displayPatchesForBrowsing", 1, param_slot_38 };
    static const QUParameter param_slot_39[] = {
	{ "vPatches", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "pos_x", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_39 = {"displayPatchesForBrowsing", 2, param_slot_39 };
    static const QUParameter param_slot_40[] = {
	{ "vPatches", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "pos_x", &static_QUType_int, 0, QUParameter::In },
	{ "pos_y", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_40 = {"displayPatchesForBrowsing", 3, param_slot_40 };
    static const QUParameter param_slot_41[] = {
	{ "idx", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_41 = {"highlightPoint", 1, param_slot_41 };
    static const QUMethod slot_42 = {"compareFeatures", 0, 0 };
    static const QUParameter param_slot_43[] = {
	{ "bShowPatches", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_43 = {"compareFeatures", 1, param_slot_43 };
    static const QUMethod slot_44 = {"testMatching", 0, 0 };
    static const QUMethod slot_45 = {"computeOccurrences", 0, 0 };
    static const QUMethod slot_46 = {"computeOccurrencesIDL", 0, 0 };
    static const QUParameter param_slot_47[] = {
	{ "qsIdlFile", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_47 = {"computeOccurrencesIDL", 1, param_slot_47 };
    static const QUMethod slot_48 = {"saveOccurrences", 0, 0 };
    static const QUMethod slot_49 = {"saveOccurrencesMatlab", 0, 0 };
    static const QUMethod slot_50 = {"loadOccurrences", 0, 0 };
    static const QUMethod slot_51 = {"loadSilhouettes", 0, 0 };
    static const QUMethod slot_52 = {"loadVerifCodebook", 0, 0 };
    static const QUMethod slot_53 = {"loadVerifOccurrences", 0, 0 };
    static const QUParameter param_slot_54[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHyposSingle", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vResultHyposTight", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vImgSegment", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "vImgPFig", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "vImgPGnd", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_54 = {"processTestImg", 8, param_slot_54 };
    static const QUMethod slot_55 = {"processTestImgStd", 0, 0 };
    static const QUParameter param_slot_56[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHyposSingle", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "ofileSingle", &static_QUType_ptr, "ofstream", QUParameter::InOut }
    };
    static const QUMethod slot_56 = {"processTestImgStd", 4, param_slot_56 };
    static const QUParameter param_slot_57[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHyposSingle", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "ofileSingle", &static_QUType_ptr, "ofstream", QUParameter::InOut },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_57 = {"processTestImgStd", 5, param_slot_57 };
    static const QUParameter param_slot_58[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHyposSingle", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "ofileSingle", &static_QUType_ptr, "ofstream", QUParameter::InOut },
	{ "ofScore", &static_QUType_ptr, "ofstream", QUParameter::InOut }
    };
    static const QUMethod slot_58 = {"processTestImgUIUC", 5, param_slot_58 };
    static const QUParameter param_slot_59[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHyposSingle", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "ofileSingle", &static_QUType_ptr, "ofstream", QUParameter::InOut },
	{ "ofScore", &static_QUType_ptr, "ofstream", QUParameter::InOut },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_59 = {"processTestImgUIUC", 6, param_slot_59 };
    static const QUParameter param_slot_60[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "idInitial", &static_QUType_ptr, "ImgDescr", QUParameter::InOut },
	{ "idTight", &static_QUType_ptr, "ImgDescr", QUParameter::InOut }
    };
    static const QUMethod slot_60 = {"processTestImgIDL", 5, param_slot_60 };
    static const QUParameter param_slot_61[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "idInitial", &static_QUType_ptr, "ImgDescr", QUParameter::InOut },
	{ "idTight", &static_QUType_ptr, "ImgDescr", QUParameter::InOut },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_61 = {"processTestImgIDL", 6, param_slot_61 };
    static const QUParameter param_slot_62[] = {
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "vResultImgSeg", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In }
    };
    static const QUMethod slot_62 = {"displayRecoResults", 2, param_slot_62 };
    static const QUParameter param_slot_63[] = {
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "vResultImgSeg", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_63 = {"displayRecoResults", 3, param_slot_63 };
    static const QUMethod slot_64 = {"getPatchHypotheses", 0, 0 };
    static const QUParameter param_slot_65[] = {
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_65 = {"getPatchHypotheses", 1, param_slot_65 };
    static const QUParameter param_slot_66[] = {
	{ "vHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "nObjWidth", &static_QUType_int, 0, QUParameter::In },
	{ "nObjHeight", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_66 = {"setHypoBBox", 3, param_slot_66 };
    static const QUParameter param_slot_67[] = {
	{ "hypo", &static_QUType_ptr, "Hypothesis", QUParameter::In },
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In }
    };
    static const QUMethod slot_67 = {"filterVotes", 2, param_slot_67 };
    static const QUParameter param_slot_68[] = {
	{ "imgSeg", &static_QUType_ptr, "OpGrayImage", QUParameter::In }
    };
    static const QUMethod slot_68 = {"computeTightBBox", 1, param_slot_68 };
    static const QUParameter param_slot_69[] = {
	{ "imgSeg", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "nOffX", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_69 = {"computeTightBBox", 2, param_slot_69 };
    static const QUParameter param_slot_70[] = {
	{ "imgSeg", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "nOffX", &static_QUType_int, 0, QUParameter::In },
	{ "nOffY", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_70 = {"computeTightBBox", 3, param_slot_70 };
    static const QUParameter param_slot_71[] = {
	{ "vInitialHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "vVerifiedHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vVerifiedHyposTight", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vResultQImgs", &static_QUType_ptr, "vector<QImage>", QUParameter::InOut }
    };
    static const QUMethod slot_71 = {"verifyHyposHarris", 4, param_slot_71 };
    static const QUParameter param_slot_72[] = {
	{ "vInitialHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "vVerifiedHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vResultQImgs", &static_QUType_ptr, "vector<QImage>", QUParameter::InOut }
    };
    static const QUMethod slot_72 = {"verifyHyposChamfer", 3, param_slot_72 };
    static const QUParameter param_slot_73[] = {
	{ "vInitHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "vInitHyposTight", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "vResultImgSeg", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In },
	{ "vResultImgPFig", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In },
	{ "vResultImgPGnd", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In },
	{ "vVerifiedHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vVerifiedHyposTight", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vVerifiedTemplates", &static_QUType_ptr, "vector<EdgePtVec>", QUParameter::InOut },
	{ "vVerifiedTemplIds", &static_QUType_ptr, "vector<int>", QUParameter::InOut },
	{ "vResultQImgs", &static_QUType_ptr, "vector<QImage>", QUParameter::InOut }
    };
    static const QUMethod slot_73 = {"verifyHyposTemplate", 10, param_slot_73 };
    static const QUParameter param_slot_74[] = {
	{ 0, &static_QUType_double, 0, QUParameter::Out },
	{ "img", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "mask", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "nShiftX", &static_QUType_int, 0, QUParameter::In },
	{ "nShiftY", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_74 = {"compBhattaMask", 5, param_slot_74 };
    static const QUMethod slot_75 = {"refineHypotheses", 0, 0 };
    static const QUParameter param_slot_76[] = {
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_76 = {"refineHypotheses", 1, param_slot_76 };
    static const QUMethod slot_77 = {"refineHypothesesMultiScale", 0, 0 };
    static const QUParameter param_slot_78[] = {
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_78 = {"refineHypothesesMultiScale", 1, param_slot_78 };
    static const QUMethod slot_79 = {"extractAllPatches", 0, 0 };
    static const QUParameter param_slot_80[] = {
	{ "process", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_80 = {"extractAllPatches", 1, param_slot_80 };
    static const QUParameter param_slot_81[] = {
	{ "page", &static_QUType_int, 0, QUParameter::In },
	{ "x", &static_QUType_int, 0, QUParameter::In },
	{ "y", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_81 = {"showSupportingPatches", 3, param_slot_81 };
    static const QUParameter param_slot_82[] = {
	{ "fvWindowPos", &static_QUType_ptr, "FeatureVector", QUParameter::In }
    };
    static const QUMethod slot_82 = {"showSupportingPatches", 1, param_slot_82 };
    static const QUParameter param_slot_83[] = {
	{ "fvWindowPos", &static_QUType_ptr, "FeatureVector", QUParameter::In },
	{ "bDrawMaps", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_83 = {"showSupportingPatches", 2, param_slot_83 };
    static const QUMethod slot_84 = {"drawMatchedPatches", 0, 0 };
    static const QUParameter param_slot_85[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In }
    };
    static const QUMethod slot_85 = {"displayVotePatches", 1, param_slot_85 };
    static const QUParameter param_slot_86[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In },
	{ "bDrawMaps", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_86 = {"displayVotePatches", 2, param_slot_86 };
    static const QUParameter param_slot_87[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In },
	{ "bDrawMaps", &static_QUType_bool, 0, QUParameter::In },
	{ "bDrawConfidence", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_87 = {"displayVotePatches", 3, param_slot_87 };
    static const QUParameter param_slot_88[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In },
	{ "bDrawMaps", &static_QUType_bool, 0, QUParameter::In },
	{ "bDrawConfidence", &static_QUType_bool, 0, QUParameter::In },
	{ "bDrawFigure", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_88 = {"displayVotePatches", 4, param_slot_88 };
    static const QUParameter param_slot_89[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In }
    };
    static const QUMethod slot_89 = {"drawVotePatches", 1, param_slot_89 };
    static const QUParameter param_slot_90[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In },
	{ "bDrawMaps", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_90 = {"drawVotePatches", 2, param_slot_90 };
    static const QUParameter param_slot_91[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In },
	{ "bDrawMaps", &static_QUType_bool, 0, QUParameter::In },
	{ "bDrawConfidence", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_91 = {"drawVotePatches", 3, param_slot_91 };
    static const QUParameter param_slot_92[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In },
	{ "bDrawMaps", &static_QUType_bool, 0, QUParameter::In },
	{ "bDrawConfidence", &static_QUType_bool, 0, QUParameter::In },
	{ "bDrawFigure", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_92 = {"drawVotePatches", 4, param_slot_92 };
    static const QUParameter param_slot_93[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In },
	{ "bDrawMaps", &static_QUType_bool, 0, QUParameter::In },
	{ "bDrawConfidence", &static_QUType_bool, 0, QUParameter::In },
	{ "bDrawFigure", &static_QUType_bool, 0, QUParameter::In },
	{ "bBackgroundZero", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_93 = {"drawVotePatches", 5, param_slot_93 };
    static const QUParameter param_slot_94[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In }
    };
    static const QUMethod slot_94 = {"displayVoteConfidence", 1, param_slot_94 };
    static const QUParameter param_slot_95[] = {
	{ "vVotes", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In },
	{ "bDrawMaps", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_95 = {"displayVoteConfidence", 2, param_slot_95 };
    static const QUMethod slot_96 = {"displayScaleFootprint", 0, 0 };
    static const QUParameter param_slot_97[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out },
	{ "ipt", &static_QUType_ptr, "InterestPoint", QUParameter::InOut }
    };
    static const QUMethod slot_97 = {"transformPoint", 2, param_slot_97 };
    static const QUParameter param_slot_98[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out },
	{ "w", &static_QUType_int, 0, QUParameter::In },
	{ "h", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_98 = {"transformPoint", 3, param_slot_98 };
    static const QUParameter param_slot_99[] = {
	{ "h1", &static_QUType_ptr, "Hypothesis", QUParameter::In },
	{ "h2", &static_QUType_ptr, "Hypothesis", QUParameter::In }
    };
    static const QUMethod slot_99 = {"computeBoundingBoxOverlap", 2, param_slot_99 };
    static const QUMethod slot_100 = {"performUIUCTestSeries", 0, 0 };
    static const QUMethod slot_101 = {"performIDLTestSeries", 0, 0 };
    static const QUMethod slot_102 = {"processImageSeries", 0, 0 };
    static const QUParameter param_slot_103[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "vImgNames", &static_QUType_ptr, "vector<string>", QUParameter::InOut },
	{ "vvAnnots", &static_QUType_ptr, "vector<vector<Hypothesis>>", QUParameter::InOut }
    };
    static const QUMethod slot_103 = {"loadExperimentFile", 3, param_slot_103 };
    static const QUMethod slot_104 = {"saveFVMatlab", 0, 0 };
    static const QUMethod slot_105 = {"saveClustersMatlab", 0, 0 };
    static const QUMethod slot_106 = {"savePatchActivationsMatlab", 0, 0 };
    static const QUMethod slot_107 = {"saveImages", 0, 0 };
    static const QUMethod slot_108 = {"saveSegmentationsMatlab", 0, 0 };
    static const QUParameter param_slot_109[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_109 = {"saveSegmentationsMatlab", 1, param_slot_109 };
    static const QUMethod slot_110 = {"saveClusterImages", 0, 0 };
    static const QUMethod slot_111 = {"saveTextureMap", 0, 0 };
    static const QUMethod slot_112 = {"saveSegmentationData", 0, 0 };
    static const QUParameter param_slot_113[] = {
	{ 0, &static_QUType_QVariant, "\x04", QUParameter::Out }
    };
    static const QUMethod slot_113 = {"getFileList", 1, param_slot_113 };
    static const QUParameter param_slot_114[] = {
	{ 0, &static_QUType_QVariant, "\x04", QUParameter::Out },
	{ "sDir", &static_QUType_ptr, "string", QUParameter::In },
	{ "sCaption", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_114 = {"getFileList", 3, param_slot_114 };
    static const QUParameter param_slot_115[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "fvVector", &static_QUType_ptr, "FeatureVector", QUParameter::InOut }
    };
    static const QUMethod slot_115 = {"readVector", 3, param_slot_115 };
    static const QUParameter param_slot_116[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "mMatrix", &static_QUType_ptr, "vector<FeatureVector>", QUParameter::InOut }
    };
    static const QUMethod slot_116 = {"readMatrix", 3, param_slot_116 };
    static const QUParameter param_slot_117[] = {
	{ "img", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_117 = {"saveImage", 2, param_slot_117 };
    static const QUParameter param_slot_118[] = {
	{ "img", &static_QUType_ptr, "GrayImage", QUParameter::In },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_118 = {"saveImageAscii", 2, param_slot_118 };
    static const QUParameter param_slot_119[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_119 = {"loadImageAscii", 1, param_slot_119 };
    static const QUParameter param_slot_120[] = {
	{ "ofile", &static_QUType_ptr, "ofstream", QUParameter::InOut },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In }
    };
    static const QUMethod slot_120 = {"writeResultsToDiskUIUC", 3, param_slot_120 };
    static const QUParameter param_slot_121[] = {
	{ "ofile", &static_QUType_ptr, "ofstream", QUParameter::InOut },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In }
    };
    static const QUMethod slot_121 = {"writeResultsToDiskScore", 3, param_slot_121 };
    static const QUMethod slot_122 = {"loadParams", 0, 0 };
    static const QUParameter param_slot_123[] = {
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_123 = {"loadParams", 1, param_slot_123 };
    static const QUParameter param_slot_124[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_124 = {"loadParams", 1, param_slot_124 };
    static const QUParameter param_slot_125[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_125 = {"loadParams", 2, param_slot_125 };
    static const QUMethod slot_126 = {"saveParams", 0, 0 };
    static const QUParameter param_slot_127[] = {
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_127 = {"saveParams", 1, param_slot_127 };
    static const QUParameter param_slot_128[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_128 = {"saveParams", 1, param_slot_128 };
    static const QUParameter param_slot_129[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_129 = {"saveParams", 2, param_slot_129 };
    static const QMetaData slot_tbl[] = {
	{ "slotSetMapThresh(const QString&)", &slot_0, QMetaData::Public },
	{ "slotUpdateMapThresh()", &slot_1, QMetaData::Public },
	{ "slotSetCateg(const QString&)", &slot_2, QMetaData::Public },
	{ "slotSetPose(const QString&)", &slot_3, QMetaData::Public },
	{ "slotSetUIUCFrom(const QString&)", &slot_4, QMetaData::Public },
	{ "slotSetObjWidth(const QString&)", &slot_5, QMetaData::Public },
	{ "slotSetObjHeight(const QString&)", &slot_6, QMetaData::Public },
	{ "slotUpdateCateg()", &slot_7, QMetaData::Public },
	{ "slotUpdatePose()", &slot_8, QMetaData::Public },
	{ "slotUpdateUIUCFrom()", &slot_9, QMetaData::Public },
	{ "slotUpdateObjWidth()", &slot_10, QMetaData::Public },
	{ "slotUpdateObjHeight()", &slot_11, QMetaData::Public },
	{ "slotSelectFixObjDim(int)", &slot_12, QMetaData::Public },
	{ "slotSetHistEqOnOff(int)", &slot_13, QMetaData::Public },
	{ "slotSetUsePatchesOnOff(int)", &slot_14, QMetaData::Public },
	{ "slotSetMapsOnOff(int)", &slot_15, QMetaData::Public },
	{ "slotSetDrawConfOnOff(int)", &slot_16, QMetaData::Public },
	{ "slotSetDrawMatchedPsOnOff(int)", &slot_17, QMetaData::Public },
	{ "slotSetFigureOnOff(int)", &slot_18, QMetaData::Public },
	{ "slotSetDrawTightBBOnOff(int)", &slot_19, QMetaData::Public },
	{ "slotSetDisplayVSOnOff(int)", &slot_20, QMetaData::Public },
	{ "slotSetDisplaySupportOnOff(int)", &slot_21, QMetaData::Public },
	{ "slotSetDisplaySegmentOnOff(int)", &slot_22, QMetaData::Public },
	{ "slotSetSavePoseOnOff(int)", &slot_23, QMetaData::Public },
	{ "slotSetRefineHypoOnOff(int)", &slot_24, QMetaData::Public },
	{ "slotSetWriteDetectOnOff(int)", &slot_25, QMetaData::Public },
	{ "slotSetDrawRejectedOnOff(int)", &slot_26, QMetaData::Public },
	{ "loadImage()", &slot_27, QMetaData::Public },
	{ "loadImage(QString)", &slot_28, QMetaData::Public },
	{ "loadImageBBox(QString,const Rect&)", &slot_29, QMetaData::Public },
	{ "loadCodebook()", &slot_30, QMetaData::Public },
	{ "loadCodebook(string)", &slot_31, QMetaData::Public },
	{ "processImage(QString)", &slot_32, QMetaData::Public },
	{ "processImageBBox(QString,const Rect&)", &slot_33, QMetaData::Public },
	{ "collectPatches()", &slot_34, QMetaData::Public },
	{ "collectPatches(bool)", &slot_35, QMetaData::Public },
	{ "drawInterestPoints()", &slot_36, QMetaData::Public },
	{ "drawInterestPointsEllipse()", &slot_37, QMetaData::Public },
	{ "displayPatchesForBrowsing(vector<OpGrayImage>&)", &slot_38, QMetaData::Public },
	{ "displayPatchesForBrowsing(vector<OpGrayImage>&,int)", &slot_39, QMetaData::Public },
	{ "displayPatchesForBrowsing(vector<OpGrayImage>&,int,int)", &slot_40, QMetaData::Public },
	{ "highlightPoint(int)", &slot_41, QMetaData::Public },
	{ "compareFeatures()", &slot_42, QMetaData::Public },
	{ "compareFeatures(bool)", &slot_43, QMetaData::Public },
	{ "testMatching()", &slot_44, QMetaData::Public },
	{ "computeOccurrences()", &slot_45, QMetaData::Public },
	{ "computeOccurrencesIDL()", &slot_46, QMetaData::Public },
	{ "computeOccurrencesIDL(const QString&)", &slot_47, QMetaData::Public },
	{ "saveOccurrences()", &slot_48, QMetaData::Public },
	{ "saveOccurrencesMatlab()", &slot_49, QMetaData::Public },
	{ "loadOccurrences()", &slot_50, QMetaData::Public },
	{ "loadSilhouettes()", &slot_51, QMetaData::Public },
	{ "loadVerifCodebook()", &slot_52, QMetaData::Public },
	{ "loadVerifOccurrences()", &slot_53, QMetaData::Public },
	{ "processTestImg(QString,int,vector<Hypothesis>&,vector<Hypothesis>&,vector<OpGrayImage>&,vector<OpGrayImage>&,vector<OpGrayImage>&,bool)", &slot_54, QMetaData::Public },
	{ "processTestImgStd()", &slot_55, QMetaData::Public },
	{ "processTestImgStd(QString,int,vector<Hypothesis>&,ofstream&)", &slot_56, QMetaData::Public },
	{ "processTestImgStd(QString,int,vector<Hypothesis>&,ofstream&,bool)", &slot_57, QMetaData::Public },
	{ "processTestImgUIUC(QString,int,vector<Hypothesis>&,ofstream&,ofstream&)", &slot_58, QMetaData::Public },
	{ "processTestImgUIUC(QString,int,vector<Hypothesis>&,ofstream&,ofstream&,bool)", &slot_59, QMetaData::Public },
	{ "processTestImgIDL(QString,int,vector<Hypothesis>&,ImgDescr&,ImgDescr&)", &slot_60, QMetaData::Public },
	{ "processTestImgIDL(QString,int,vector<Hypothesis>&,ImgDescr&,ImgDescr&,bool)", &slot_61, QMetaData::Public },
	{ "displayRecoResults(const vector<Hypothesis>&,const vector<OpGrayImage>&)", &slot_62, QMetaData::Public },
	{ "displayRecoResults(const vector<Hypothesis>&,const vector<OpGrayImage>&,bool)", &slot_63, QMetaData::Public },
	{ "getPatchHypotheses()", &slot_64, QMetaData::Public },
	{ "getPatchHypotheses(bool)", &slot_65, QMetaData::Public },
	{ "setHypoBBox(const vector<Hypothesis>&,int,int)", &slot_66, QMetaData::Public },
	{ "filterVotes(const Hypothesis&,const vector<HoughVote>&)", &slot_67, QMetaData::Public },
	{ "computeTightBBox(OpGrayImage)", &slot_68, QMetaData::Public },
	{ "computeTightBBox(OpGrayImage,int)", &slot_69, QMetaData::Public },
	{ "computeTightBBox(OpGrayImage,int,int)", &slot_70, QMetaData::Public },
	{ "verifyHyposHarris(const vector<Hypothesis>&,vector<Hypothesis>&,vector<Hypothesis>&,vector<QImage>&)", &slot_71, QMetaData::Public },
	{ "verifyHyposChamfer(const vector<Hypothesis>&,vector<Hypothesis>&,vector<QImage>&)", &slot_72, QMetaData::Public },
	{ "verifyHyposTemplate(const vector<Hypothesis>&,const vector<Hypothesis>&,const vector<OpGrayImage>&,const vector<OpGrayImage>&,const vector<OpGrayImage>&,vector<Hypothesis>&,vector<Hypothesis>&,vector<EdgePtVec>&,vector<int>&,vector<QImage>&)", &slot_73, QMetaData::Public },
	{ "compBhattaMask(const OpGrayImage&,const OpGrayImage&,int,int)", &slot_74, QMetaData::Public },
	{ "refineHypotheses()", &slot_75, QMetaData::Public },
	{ "refineHypotheses(bool)", &slot_76, QMetaData::Public },
	{ "refineHypothesesMultiScale()", &slot_77, QMetaData::Public },
	{ "refineHypothesesMultiScale(bool)", &slot_78, QMetaData::Public },
	{ "extractAllPatches()", &slot_79, QMetaData::Public },
	{ "extractAllPatches(bool)", &slot_80, QMetaData::Public },
	{ "showSupportingPatches(int,int,int)", &slot_81, QMetaData::Public },
	{ "showSupportingPatches(FeatureVector)", &slot_82, QMetaData::Public },
	{ "showSupportingPatches(FeatureVector,bool)", &slot_83, QMetaData::Public },
	{ "drawMatchedPatches()", &slot_84, QMetaData::Public },
	{ "displayVotePatches(vector<HoughVote>)", &slot_85, QMetaData::Public },
	{ "displayVotePatches(vector<HoughVote>,bool)", &slot_86, QMetaData::Public },
	{ "displayVotePatches(vector<HoughVote>,bool,bool)", &slot_87, QMetaData::Public },
	{ "displayVotePatches(vector<HoughVote>,bool,bool,bool)", &slot_88, QMetaData::Public },
	{ "drawVotePatches(vector<HoughVote>)", &slot_89, QMetaData::Public },
	{ "drawVotePatches(vector<HoughVote>,bool)", &slot_90, QMetaData::Public },
	{ "drawVotePatches(vector<HoughVote>,bool,bool)", &slot_91, QMetaData::Public },
	{ "drawVotePatches(vector<HoughVote>,bool,bool,bool)", &slot_92, QMetaData::Public },
	{ "drawVotePatches(vector<HoughVote>,bool,bool,bool,bool)", &slot_93, QMetaData::Public },
	{ "displayVoteConfidence(vector<HoughVote>)", &slot_94, QMetaData::Public },
	{ "displayVoteConfidence(vector<HoughVote>,bool)", &slot_95, QMetaData::Public },
	{ "displayScaleFootprint()", &slot_96, QMetaData::Public },
	{ "transformPoint(InterestPoint&)", &slot_97, QMetaData::Public },
	{ "transformPoint(int,int)", &slot_98, QMetaData::Public },
	{ "computeBoundingBoxOverlap(Hypothesis,Hypothesis)", &slot_99, QMetaData::Public },
	{ "performUIUCTestSeries()", &slot_100, QMetaData::Public },
	{ "performIDLTestSeries()", &slot_101, QMetaData::Public },
	{ "processImageSeries()", &slot_102, QMetaData::Public },
	{ "loadExperimentFile(QString,vector<string>&,vector<vector<Hypothesis>>&)", &slot_103, QMetaData::Public },
	{ "saveFVMatlab()", &slot_104, QMetaData::Public },
	{ "saveClustersMatlab()", &slot_105, QMetaData::Public },
	{ "savePatchActivationsMatlab()", &slot_106, QMetaData::Public },
	{ "saveImages()", &slot_107, QMetaData::Public },
	{ "saveSegmentationsMatlab()", &slot_108, QMetaData::Public },
	{ "saveSegmentationsMatlab(QString)", &slot_109, QMetaData::Public },
	{ "saveClusterImages()", &slot_110, QMetaData::Public },
	{ "saveTextureMap()", &slot_111, QMetaData::Public },
	{ "saveSegmentationData()", &slot_112, QMetaData::Public },
	{ "getFileList()", &slot_113, QMetaData::Public },
	{ "getFileList(string,string)", &slot_114, QMetaData::Public },
	{ "readVector(string,FeatureVector&)", &slot_115, QMetaData::Public },
	{ "readMatrix(string,vector<FeatureVector>&)", &slot_116, QMetaData::Public },
	{ "saveImage(OpGrayImage,string)", &slot_117, QMetaData::Public },
	{ "saveImageAscii(GrayImage,string)", &slot_118, QMetaData::Public },
	{ "loadImageAscii(string)", &slot_119, QMetaData::Public },
	{ "writeResultsToDiskUIUC(ofstream&,int,vector<Hypothesis>)", &slot_120, QMetaData::Public },
	{ "writeResultsToDiskScore(ofstream&,int,vector<Hypothesis>)", &slot_121, QMetaData::Public },
	{ "loadParams()", &slot_122, QMetaData::Public },
	{ "loadParams(bool)", &slot_123, QMetaData::Public },
	{ "loadParams(string)", &slot_124, QMetaData::Public },
	{ "loadParams(string,bool)", &slot_125, QMetaData::Public },
	{ "saveParams()", &slot_126, QMetaData::Public },
	{ "saveParams(bool)", &slot_127, QMetaData::Public },
	{ "saveParams(string)", &slot_128, QMetaData::Public },
	{ "saveParams(string,bool)", &slot_129, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigMapThreshChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"sigAgreeingChanged", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"sigFromChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"sigToChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"sigCategChanged", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"sigPoseChanged", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_6 = {"sigUIUCFromChanged", 1, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"sigObjWidthChanged", 1, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_8 = {"sigObjHeightChanged", 1, param_signal_8 };
    static const QMetaData signal_tbl[] = {
	{ "sigMapThreshChanged(const QString&)", &signal_0, QMetaData::Public },
	{ "sigAgreeingChanged(const QString&)", &signal_1, QMetaData::Public },
	{ "sigFromChanged(const QString&)", &signal_2, QMetaData::Public },
	{ "sigToChanged(const QString&)", &signal_3, QMetaData::Public },
	{ "sigCategChanged(const QString&)", &signal_4, QMetaData::Public },
	{ "sigPoseChanged(const QString&)", &signal_5, QMetaData::Public },
	{ "sigUIUCFromChanged(const QString&)", &signal_6, QMetaData::Public },
	{ "sigObjWidthChanged(const QString&)", &signal_7, QMetaData::Public },
	{ "sigObjHeightChanged(const QString&)", &signal_8, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ISMReco", parentObject,
	slot_tbl, 130,
	signal_tbl, 9,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ISMReco.setMetaObject( metaObj );
    return metaObj;
}

void* ISMReco::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ISMReco" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sigMapThreshChanged
void ISMReco::sigMapThreshChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL sigAgreeingChanged
void ISMReco::sigAgreeingChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL sigFromChanged
void ISMReco::sigFromChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL sigToChanged
void ISMReco::sigToChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL sigCategChanged
void ISMReco::sigCategChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL sigPoseChanged
void ISMReco::sigPoseChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL sigUIUCFromChanged
void ISMReco::sigUIUCFromChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 6, t0 );
}

// SIGNAL sigObjWidthChanged
void ISMReco::sigObjWidthChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 7, t0 );
}

// SIGNAL sigObjHeightChanged
void ISMReco::sigObjHeightChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 8, t0 );
}

bool ISMReco::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotSetMapThresh((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: slotUpdateMapThresh(); break;
    case 2: slotSetCateg((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: slotSetPose((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: slotSetUIUCFrom((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: slotSetObjWidth((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: slotSetObjHeight((const QString&)static_QUType_QString.get(_o+1)); break;
    case 7: slotUpdateCateg(); break;
    case 8: slotUpdatePose(); break;
    case 9: slotUpdateUIUCFrom(); break;
    case 10: slotUpdateObjWidth(); break;
    case 11: slotUpdateObjHeight(); break;
    case 12: slotSelectFixObjDim((int)static_QUType_int.get(_o+1)); break;
    case 13: slotSetHistEqOnOff((int)static_QUType_int.get(_o+1)); break;
    case 14: slotSetUsePatchesOnOff((int)static_QUType_int.get(_o+1)); break;
    case 15: slotSetMapsOnOff((int)static_QUType_int.get(_o+1)); break;
    case 16: slotSetDrawConfOnOff((int)static_QUType_int.get(_o+1)); break;
    case 17: slotSetDrawMatchedPsOnOff((int)static_QUType_int.get(_o+1)); break;
    case 18: slotSetFigureOnOff((int)static_QUType_int.get(_o+1)); break;
    case 19: slotSetDrawTightBBOnOff((int)static_QUType_int.get(_o+1)); break;
    case 20: slotSetDisplayVSOnOff((int)static_QUType_int.get(_o+1)); break;
    case 21: slotSetDisplaySupportOnOff((int)static_QUType_int.get(_o+1)); break;
    case 22: slotSetDisplaySegmentOnOff((int)static_QUType_int.get(_o+1)); break;
    case 23: slotSetSavePoseOnOff((int)static_QUType_int.get(_o+1)); break;
    case 24: slotSetRefineHypoOnOff((int)static_QUType_int.get(_o+1)); break;
    case 25: slotSetWriteDetectOnOff((int)static_QUType_int.get(_o+1)); break;
    case 26: slotSetDrawRejectedOnOff((int)static_QUType_int.get(_o+1)); break;
    case 27: loadImage(); break;
    case 28: loadImage((QString)static_QUType_QString.get(_o+1)); break;
    case 29: loadImageBBox((QString)static_QUType_QString.get(_o+1),(const Rect&)*((const Rect*)static_QUType_ptr.get(_o+2))); break;
    case 30: loadCodebook(); break;
    case 31: loadCodebook((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 32: processImage((QString)static_QUType_QString.get(_o+1)); break;
    case 33: processImageBBox((QString)static_QUType_QString.get(_o+1),(const Rect&)*((const Rect*)static_QUType_ptr.get(_o+2))); break;
    case 34: collectPatches(); break;
    case 35: collectPatches((bool)static_QUType_bool.get(_o+1)); break;
    case 36: drawInterestPoints(); break;
    case 37: drawInterestPointsEllipse(); break;
    case 38: displayPatchesForBrowsing((vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+1))); break;
    case 39: displayPatchesForBrowsing((vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2)); break;
    case 40: displayPatchesForBrowsing((vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3)); break;
    case 41: highlightPoint((int)static_QUType_int.get(_o+1)); break;
    case 42: compareFeatures(); break;
    case 43: compareFeatures((bool)static_QUType_bool.get(_o+1)); break;
    case 44: testMatching(); break;
    case 45: computeOccurrences(); break;
    case 46: computeOccurrencesIDL(); break;
    case 47: computeOccurrencesIDL((const QString&)static_QUType_QString.get(_o+1)); break;
    case 48: saveOccurrences(); break;
    case 49: saveOccurrencesMatlab(); break;
    case 50: loadOccurrences(); break;
    case 51: loadSilhouettes(); break;
    case 52: loadVerifCodebook(); break;
    case 53: loadVerifOccurrences(); break;
    case 54: processTestImg((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+4)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+5)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+6)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+7)),(bool)static_QUType_bool.get(_o+8)); break;
    case 55: processTestImgStd(); break;
    case 56: processTestImgStd((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(ofstream&)*((ofstream*)static_QUType_ptr.get(_o+4))); break;
    case 57: processTestImgStd((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(ofstream&)*((ofstream*)static_QUType_ptr.get(_o+4)),(bool)static_QUType_bool.get(_o+5)); break;
    case 58: processTestImgUIUC((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(ofstream&)*((ofstream*)static_QUType_ptr.get(_o+4)),(ofstream&)*((ofstream*)static_QUType_ptr.get(_o+5))); break;
    case 59: processTestImgUIUC((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(ofstream&)*((ofstream*)static_QUType_ptr.get(_o+4)),(ofstream&)*((ofstream*)static_QUType_ptr.get(_o+5)),(bool)static_QUType_bool.get(_o+6)); break;
    case 60: processTestImgIDL((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(ImgDescr&)*((ImgDescr*)static_QUType_ptr.get(_o+4)),(ImgDescr&)*((ImgDescr*)static_QUType_ptr.get(_o+5))); break;
    case 61: processTestImgIDL((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(ImgDescr&)*((ImgDescr*)static_QUType_ptr.get(_o+4)),(ImgDescr&)*((ImgDescr*)static_QUType_ptr.get(_o+5)),(bool)static_QUType_bool.get(_o+6)); break;
    case 62: displayRecoResults((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+2))); break;
    case 63: displayRecoResults((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+2)),(bool)static_QUType_bool.get(_o+3)); break;
    case 64: getPatchHypotheses(); break;
    case 65: getPatchHypotheses((bool)static_QUType_bool.get(_o+1)); break;
    case 66: setHypoBBox((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3)); break;
    case 67: filterVotes((const Hypothesis&)*((const Hypothesis*)static_QUType_ptr.get(_o+1)),(const vector<HoughVote>&)*((const vector<HoughVote>*)static_QUType_ptr.get(_o+2))); break;
    case 68: computeTightBBox((OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+1)))); break;
    case 69: computeTightBBox((OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+1))),(int)static_QUType_int.get(_o+2)); break;
    case 70: computeTightBBox((OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+1))),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3)); break;
    case 71: verifyHyposHarris((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+2)),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(vector<QImage>&)*((vector<QImage>*)static_QUType_ptr.get(_o+4))); break;
    case 72: verifyHyposChamfer((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+2)),(vector<QImage>&)*((vector<QImage>*)static_QUType_ptr.get(_o+3))); break;
    case 73: verifyHyposTemplate((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+2)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+3)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+4)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+5)),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+6)),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+7)),(vector<EdgePtVec>&)*((vector<EdgePtVec>*)static_QUType_ptr.get(_o+8)),(vector<int>&)*((vector<int>*)static_QUType_ptr.get(_o+9)),(vector<QImage>&)*((vector<QImage>*)static_QUType_ptr.get(_o+10))); break;
    case 74: static_QUType_double.set(_o,compBhattaMask((const OpGrayImage&)*((const OpGrayImage*)static_QUType_ptr.get(_o+1)),(const OpGrayImage&)*((const OpGrayImage*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3),(int)static_QUType_int.get(_o+4))); break;
    case 75: refineHypotheses(); break;
    case 76: refineHypotheses((bool)static_QUType_bool.get(_o+1)); break;
    case 77: refineHypothesesMultiScale(); break;
    case 78: refineHypothesesMultiScale((bool)static_QUType_bool.get(_o+1)); break;
    case 79: extractAllPatches(); break;
    case 80: extractAllPatches((bool)static_QUType_bool.get(_o+1)); break;
    case 81: showSupportingPatches((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3)); break;
    case 82: showSupportingPatches((FeatureVector)(*((FeatureVector*)static_QUType_ptr.get(_o+1)))); break;
    case 83: showSupportingPatches((FeatureVector)(*((FeatureVector*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 84: drawMatchedPatches(); break;
    case 85: displayVotePatches((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1)))); break;
    case 86: displayVotePatches((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 87: displayVotePatches((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2),(bool)static_QUType_bool.get(_o+3)); break;
    case 88: displayVotePatches((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2),(bool)static_QUType_bool.get(_o+3),(bool)static_QUType_bool.get(_o+4)); break;
    case 89: drawVotePatches((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1)))); break;
    case 90: drawVotePatches((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 91: drawVotePatches((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2),(bool)static_QUType_bool.get(_o+3)); break;
    case 92: drawVotePatches((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2),(bool)static_QUType_bool.get(_o+3),(bool)static_QUType_bool.get(_o+4)); break;
    case 93: drawVotePatches((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2),(bool)static_QUType_bool.get(_o+3),(bool)static_QUType_bool.get(_o+4),(bool)static_QUType_bool.get(_o+5)); break;
    case 94: displayVoteConfidence((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1)))); break;
    case 95: displayVoteConfidence((vector<HoughVote>)(*((vector<HoughVote>*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 96: displayScaleFootprint(); break;
    case 97: static_QUType_int.set(_o,transformPoint((InterestPoint&)*((InterestPoint*)static_QUType_ptr.get(_o+1)))); break;
    case 98: static_QUType_int.set(_o,transformPoint((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2))); break;
    case 99: computeBoundingBoxOverlap((Hypothesis)(*((Hypothesis*)static_QUType_ptr.get(_o+1))),(Hypothesis)(*((Hypothesis*)static_QUType_ptr.get(_o+2)))); break;
    case 100: performUIUCTestSeries(); break;
    case 101: performIDLTestSeries(); break;
    case 102: processImageSeries(); break;
    case 103: loadExperimentFile((QString)static_QUType_QString.get(_o+1),(vector<string>&)*((vector<string>*)static_QUType_ptr.get(_o+2)),(vector<vector<Hypothesis> >&)*((vector<vector<Hypothesis> >*)static_QUType_ptr.get(_o+3))); break;
    case 104: saveFVMatlab(); break;
    case 105: saveClustersMatlab(); break;
    case 106: savePatchActivationsMatlab(); break;
    case 107: saveImages(); break;
    case 108: saveSegmentationsMatlab(); break;
    case 109: saveSegmentationsMatlab((QString)static_QUType_QString.get(_o+1)); break;
    case 110: saveClusterImages(); break;
    case 111: saveTextureMap(); break;
    case 112: saveSegmentationData(); break;
    case 113: static_QUType_QVariant.set(_o,QVariant(getFileList())); break;
    case 114: static_QUType_QVariant.set(_o,QVariant(getFileList((string)(*((string*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2)))))); break;
    case 115: static_QUType_bool.set(_o,readVector((string)(*((string*)static_QUType_ptr.get(_o+1))),(FeatureVector&)*((FeatureVector*)static_QUType_ptr.get(_o+2)))); break;
    case 116: static_QUType_bool.set(_o,readMatrix((string)(*((string*)static_QUType_ptr.get(_o+1))),(vector<FeatureVector>&)*((vector<FeatureVector>*)static_QUType_ptr.get(_o+2)))); break;
    case 117: saveImage((OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2)))); break;
    case 118: saveImageAscii((GrayImage)(*((GrayImage*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2)))); break;
    case 119: loadImageAscii((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 120: writeResultsToDiskUIUC((ofstream&)*((ofstream*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>)(*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)))); break;
    case 121: writeResultsToDiskScore((ofstream&)*((ofstream*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>)(*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)))); break;
    case 122: loadParams(); break;
    case 123: loadParams((bool)static_QUType_bool.get(_o+1)); break;
    case 124: loadParams((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 125: loadParams((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 126: saveParams(); break;
    case 127: saveParams((bool)static_QUType_bool.get(_o+1)); break;
    case 128: saveParams((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 129: saveParams((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ISMReco::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigMapThreshChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: sigAgreeingChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: sigFromChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: sigToChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: sigCategChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: sigPoseChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: sigUIUCFromChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 7: sigObjWidthChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: sigObjHeightChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ISMReco::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool ISMReco::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
