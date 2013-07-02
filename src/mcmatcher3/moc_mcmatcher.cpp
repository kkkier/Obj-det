/****************************************************************************
** ISMReco meta object code from reading C++ file 'mcmatcher.hh'
**
** Created: Tue Dec 25 06:30:20 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mcmatcher.hh"
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
    static const QUParameter param_slot_1[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotSetImgScale", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"slotSetWorldScale", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"slotSetMaxOverlap", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotSetMinPFig", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"slotSetWeightPFig", 1, param_slot_5 };
    static const QUMethod slot_6 = {"slotUpdateImgScale", 0, 0 };
    static const QUMethod slot_7 = {"slotUpdateWorldScale", 0, 0 };
    static const QUMethod slot_8 = {"slotUpdateMapThresh", 0, 0 };
    static const QUMethod slot_9 = {"slotUpdateMaxOverlap", 0, 0 };
    static const QUMethod slot_10 = {"slotUpdateMinPFig", 0, 0 };
    static const QUMethod slot_11 = {"slotUpdateWeightPFig", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"slotSelectGPFrame", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"slotSetGrayConvOnOff", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"slotSetGammaNormOnOff", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"slotSetUsePatchesOnOff", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"slotSetDrawMapsOnOff", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"slotSetDrawConfOnOff", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"slotSetShowTxtStepsOnOff", 1, param_slot_18 };
    static const QUParameter param_slot_19[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_19 = {"slotSetShowTxtDetailsOnOff", 1, param_slot_19 };
    static const QUParameter param_slot_20[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"slotSetShowTxtVotingOnOff", 1, param_slot_20 };
    static const QUParameter param_slot_21[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_21 = {"slotSetShowTxtMDLOnOff", 1, param_slot_21 };
    static const QUParameter param_slot_22[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_22 = {"slotSetShowTimingsOnOff", 1, param_slot_22 };
    static const QUParameter param_slot_23[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_23 = {"slotSetShowIntPtsOnOff", 1, param_slot_23 };
    static const QUParameter param_slot_24[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_24 = {"slotSetFigureOnOff", 1, param_slot_24 };
    static const QUParameter param_slot_25[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_25 = {"slotSetDrawTightBBOnOff", 1, param_slot_25 };
    static const QUParameter param_slot_26[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_26 = {"slotSetEvalUniqueOnOff", 1, param_slot_26 };
    static const QUParameter param_slot_27[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_27 = {"slotSetDisplayVSOnOff", 1, param_slot_27 };
    static const QUParameter param_slot_28[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_28 = {"slotSetDisplaySupportOnOff", 1, param_slot_28 };
    static const QUParameter param_slot_29[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_29 = {"slotSetDisplaySegmentOnOff", 1, param_slot_29 };
    static const QUParameter param_slot_30[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_30 = {"slotSetDisplayMultiCueOnOff", 1, param_slot_30 };
    static const QUParameter param_slot_31[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_31 = {"slotSetWriteResultsOnOff", 1, param_slot_31 };
    static const QUParameter param_slot_32[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_32 = {"slotSetWriteSegsOnOff", 1, param_slot_32 };
    static const QUParameter param_slot_33[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_33 = {"slotSetDoMDLOnOff", 1, param_slot_33 };
    static const QUParameter param_slot_34[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_34 = {"slotSetRejectOverlapOnOff", 1, param_slot_34 };
    static const QUParameter param_slot_35[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_35 = {"slotSetRejectPFigOnOff", 1, param_slot_35 };
    static const QUParameter param_slot_36[] = {
	{ "bShowGUI", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_36 = {"setShowGUI", 1, param_slot_36 };
    static const QUMethod slot_37 = {"loadImage", 0, 0 };
    static const QUParameter param_slot_38[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_38 = {"loadImage", 1, param_slot_38 };
    static const QUMethod slot_39 = {"drawGroundPlane", 0, 0 };
    static const QUParameter param_slot_40[] = {
	{ "x1", &static_QUType_ptr, "float", QUParameter::In },
	{ "x2", &static_QUType_ptr, "float", QUParameter::In },
	{ "d", &static_QUType_ptr, "float", QUParameter::In },
	{ "h", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_40 = {"drawDistHLine", 4, param_slot_40 };
    static const QUParameter param_slot_41[] = {
	{ "x1", &static_QUType_ptr, "float", QUParameter::In },
	{ "x2", &static_QUType_ptr, "float", QUParameter::In },
	{ "d", &static_QUType_ptr, "float", QUParameter::In },
	{ "h", &static_QUType_ptr, "float", QUParameter::In },
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_41 = {"drawDistHLine", 5, param_slot_41 };
    static const QUParameter param_slot_42[] = {
	{ "x", &static_QUType_ptr, "float", QUParameter::In },
	{ "d1", &static_QUType_ptr, "float", QUParameter::In },
	{ "d2", &static_QUType_ptr, "float", QUParameter::In },
	{ "h", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_42 = {"drawDistVLine", 4, param_slot_42 };
    static const QUParameter param_slot_43[] = {
	{ "x", &static_QUType_ptr, "float", QUParameter::In },
	{ "d1", &static_QUType_ptr, "float", QUParameter::In },
	{ "d2", &static_QUType_ptr, "float", QUParameter::In },
	{ "h", &static_QUType_ptr, "float", QUParameter::In },
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_43 = {"drawDistVLine", 5, param_slot_43 };
    static const QUParameter param_slot_44[] = {
	{ "x", &static_QUType_ptr, "float", QUParameter::In },
	{ "d", &static_QUType_ptr, "float", QUParameter::In },
	{ "h1", &static_QUType_ptr, "float", QUParameter::In },
	{ "h2", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_44 = {"drawDistZLine", 4, param_slot_44 };
    static const QUParameter param_slot_45[] = {
	{ "x", &static_QUType_ptr, "float", QUParameter::In },
	{ "d", &static_QUType_ptr, "float", QUParameter::In },
	{ "h1", &static_QUType_ptr, "float", QUParameter::In },
	{ "h2", &static_QUType_ptr, "float", QUParameter::In },
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_45 = {"drawDistZLine", 5, param_slot_45 };
    static const QUParameter param_slot_46[] = {
	{ "dummy1", &static_QUType_int, 0, QUParameter::In },
	{ "dummy2", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_46 = {"showResultImg", 2, param_slot_46 };
    static const QUMethod slot_47 = {"showResultImg", 0, 0 };
    static const QUMethod slot_48 = {"addDetector", 0, 0 };
    static const QUParameter param_slot_49[] = {
	{ "sDetFile", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_49 = {"addDetector", 1, param_slot_49 };
    static const QUParameter param_slot_50[] = {
	{ "sDetFile", &static_QUType_QString, 0, QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_50 = {"addDetector", 2, param_slot_50 };
    static const QUParameter param_slot_51[] = {
	{ "nRow", &static_QUType_int, 0, QUParameter::In },
	{ "nCol", &static_QUType_int, 0, QUParameter::In },
	{ "button", &static_QUType_int, 0, QUParameter::In },
	{ "mousePos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_51 = {"detectorTableClicked", 4, param_slot_51 };
    static const QUParameter param_slot_52[] = {
	{ "nIdx", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_52 = {"showDetectorWindow", 1, param_slot_52 };
    static const QUParameter param_slot_53[] = {
	{ "sCategory", &static_QUType_ptr, "string", QUParameter::In },
	{ "sPose", &static_QUType_ptr, "string", QUParameter::In },
	{ "sFlipped", &static_QUType_ptr, "string", QUParameter::In },
	{ "sThresh", &static_QUType_ptr, "string", QUParameter::In },
	{ "sColor", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_53 = {"addDetectorTableEntry", 5, param_slot_53 };
    static const QUMethod slot_54 = {"updateDetectorTable", 0, 0 };
    static const QUParameter param_slot_55[] = {
	{ "nRow", &static_QUType_int, 0, QUParameter::In },
	{ "nCol", &static_QUType_int, 0, QUParameter::In },
	{ "button", &static_QUType_int, 0, QUParameter::In },
	{ "mousePos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_55 = {"cueTableClicked", 4, param_slot_55 };
    static const QUParameter param_slot_56[] = {
	{ "nIdx", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_56 = {"showCueWindow", 1, param_slot_56 };
    static const QUParameter param_slot_57[] = {
	{ "sDetector", &static_QUType_ptr, "string", QUParameter::In },
	{ "sFeature", &static_QUType_ptr, "string", QUParameter::In },
	{ "sFlipped", &static_QUType_ptr, "string", QUParameter::In },
	{ "sNumCl", &static_QUType_ptr, "string", QUParameter::In },
	{ "sNumOcc", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_57 = {"addCueTableEntry", 5, param_slot_57 };
    static const QUParameter param_slot_58[] = {
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "sDetector", &static_QUType_ptr, "string", QUParameter::In },
	{ "sFeature", &static_QUType_ptr, "string", QUParameter::In },
	{ "sFlipped", &static_QUType_ptr, "string", QUParameter::In },
	{ "sNumCl", &static_QUType_ptr, "string", QUParameter::In },
	{ "sNumOcc", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_58 = {"updateCueTableEntry", 6, param_slot_58 };
    static const QUMethod slot_59 = {"collectCueInformation", 0, 0 };
    static const QUParameter param_slot_60[] = {
	{ "qstr", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_60 = {"processImage", 1, param_slot_60 };
    static const QUMethod slot_61 = {"collectPatches", 0, 0 };
    static const QUParameter param_slot_62[] = {
	{ "process", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_62 = {"collectPatches", 1, param_slot_62 };
    static const QUMethod slot_63 = {"drawInterestPoints", 0, 0 };
    static const QUParameter param_slot_64[] = {
	{ "dScaleFactor", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod slot_64 = {"drawInterestPointsEllipse", 1, param_slot_64 };
    static const QUParameter param_slot_65[] = {
	{ "vPatches", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut }
    };
    static const QUMethod slot_65 = {"displayPatchesForBrowsing", 1, param_slot_65 };
    static const QUParameter param_slot_66[] = {
	{ "vPatches", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "pos_x", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_66 = {"displayPatchesForBrowsing", 2, param_slot_66 };
    static const QUParameter param_slot_67[] = {
	{ "vPatches", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "pos_x", &static_QUType_int, 0, QUParameter::In },
	{ "pos_y", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_67 = {"displayPatchesForBrowsing", 3, param_slot_67 };
    static const QUMethod slot_68 = {"compareFeatures", 0, 0 };
    static const QUParameter param_slot_69[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHyposSingle", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vResultHyposTight", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vImgSegment", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "vImgPFig", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "vImgPGnd", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_69 = {"processTestImg", 8, param_slot_69 };
    static const QUParameter param_slot_70[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHyposSingle", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vResultHyposTight", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vImgSegment", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "vImgPFig", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "vImgPGnd", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_70 = {"processTestImgBothDir", 8, param_slot_70 };
    static const QUMethod slot_71 = {"processTestImgStd", 0, 0 };
    static const QUParameter param_slot_72[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_72 = {"processTestImgStd", 1, param_slot_72 };
    static const QUParameter param_slot_73[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHyposSingle", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vImgSegment", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "vImgPFig", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "vImgPGnd", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "ofileSingle", &static_QUType_ptr, "ofstream", QUParameter::InOut }
    };
    static const QUMethod slot_73 = {"processTestImgStd", 7, param_slot_73 };
    static const QUParameter param_slot_74[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vResultHyposSingle", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "vImgSegment", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "vImgPFig", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "vImgPGnd", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut },
	{ "ofileSingle", &static_QUType_ptr, "ofstream", QUParameter::InOut },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_74 = {"processTestImgStd", 8, param_slot_74 };
    static const QUParameter param_slot_75[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "qsResultDir", &static_QUType_QString, 0, QUParameter::In },
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "idInitial", &static_QUType_ptr, "ImgDescr", QUParameter::InOut },
	{ "idTight", &static_QUType_ptr, "ImgDescr", QUParameter::InOut }
    };
    static const QUMethod slot_75 = {"processTestImgIDL", 6, param_slot_75 };
    static const QUParameter param_slot_76[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "qsResultDir", &static_QUType_QString, 0, QUParameter::In },
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::InOut },
	{ "idInitial", &static_QUType_ptr, "ImgDescr", QUParameter::InOut },
	{ "idTight", &static_QUType_ptr, "ImgDescr", QUParameter::InOut },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_76 = {"processTestImgIDL", 7, param_slot_76 };
    static const QUParameter param_slot_77[] = {
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "vResultImgSeg", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In },
	{ "vResultImgPFig", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In },
	{ "vResultImgPGnd", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In }
    };
    static const QUMethod slot_77 = {"displayRecoResults", 4, param_slot_77 };
    static const QUParameter param_slot_78[] = {
	{ "vResultHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In },
	{ "vResultImgSeg", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In },
	{ "vResultImgPFig", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In },
	{ "vResultImgPGnd", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::In },
	{ "bDisplayResults", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_78 = {"displayRecoResults", 5, param_slot_78 };
    static const QUParameter param_slot_79[] = {
	{ "imgSeg", &static_QUType_ptr, "OpGrayImage", QUParameter::In }
    };
    static const QUMethod slot_79 = {"computeTightBBox", 1, param_slot_79 };
    static const QUParameter param_slot_80[] = {
	{ "imgSeg", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "nOffX", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_80 = {"computeTightBBox", 2, param_slot_80 };
    static const QUParameter param_slot_81[] = {
	{ "imgSeg", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "nOffX", &static_QUType_int, 0, QUParameter::In },
	{ "nOffY", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_81 = {"computeTightBBox", 3, param_slot_81 };
    static const QUParameter param_slot_82[] = {
	{ "hypo", &static_QUType_ptr, "Hypothesis", QUParameter::InOut },
	{ "nIdx", &static_QUType_ptr, "unsigned", QUParameter::In },
	{ "vSupporting", &static_QUType_ptr, "vector<HoughVote>", QUParameter::In }
    };
    static const QUMethod slot_82 = {"countPatches", 3, param_slot_82 };
    static const QUParameter param_slot_83[] = {
	{ "hypo", &static_QUType_ptr, "Hypothesis", QUParameter::InOut },
	{ "segResult", &static_QUType_ptr, "Segmentation", QUParameter::InOut }
    };
    static const QUMethod slot_83 = {"resampleHypothesis", 2, param_slot_83 };
    static const QUMethod slot_84 = {"loadSilhouettes", 0, 0 };
    static const QUParameter param_slot_85[] = {
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
    static const QUMethod slot_85 = {"verifyHyposTemplate", 10, param_slot_85 };
    static const QUParameter param_slot_86[] = {
	{ 0, &static_QUType_double, 0, QUParameter::Out },
	{ "img", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "mask", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "nShiftX", &static_QUType_int, 0, QUParameter::In },
	{ "nShiftY", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_86 = {"compBhattaMask", 5, param_slot_86 };
    static const QUParameter param_slot_87[] = {
	{ "hypo", &static_QUType_ptr, "Hypothesis", QUParameter::In },
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In },
	{ "bPermanent", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_87 = {"drawHypothesis", 3, param_slot_87 };
    static const QUMethod slot_88 = {"refreshSrcImg", 0, 0 };
    static const QUParameter param_slot_89[] = {
	{ "qimg", &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod slot_89 = {"displayResultImg", 1, param_slot_89 };
    static const QUParameter param_slot_90[] = {
	{ "qimg", &static_QUType_varptr, "\x0f", QUParameter::In },
	{ "img", &static_QUType_ptr, "OpGrayImage", QUParameter::In }
    };
    static const QUMethod slot_90 = {"displayResultImg", 2, param_slot_90 };
    static const QUMethod slot_91 = {"displayScaleFootprint", 0, 0 };
    static const QUParameter param_slot_92[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out },
	{ "ipt", &static_QUType_ptr, "InterestPoint", QUParameter::InOut }
    };
    static const QUMethod slot_92 = {"transformPoint", 2, param_slot_92 };
    static const QUParameter param_slot_93[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out },
	{ "w", &static_QUType_int, 0, QUParameter::In },
	{ "h", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_93 = {"transformPoint", 3, param_slot_93 };
    static const QUParameter param_slot_94[] = {
	{ "h1", &static_QUType_ptr, "Hypothesis", QUParameter::In },
	{ "h2", &static_QUType_ptr, "Hypothesis", QUParameter::In }
    };
    static const QUMethod slot_94 = {"computeBoundingBoxOverlap", 2, param_slot_94 };
    static const QUMethod slot_95 = {"performIDLTestSeries", 0, 0 };
    static const QUParameter param_slot_96[] = {
	{ "qsExpFile", &static_QUType_QString, 0, QUParameter::In },
	{ "qsResultFile1", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_96 = {"performIDLTestSeries", 2, param_slot_96 };
    static const QUParameter param_slot_97[] = {
	{ "qsExpFile", &static_QUType_QString, 0, QUParameter::In },
	{ "qsResultFile1", &static_QUType_QString, 0, QUParameter::In },
	{ "qsSaveDirName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_97 = {"performIDLTestSeries", 3, param_slot_97 };
    static const QUParameter param_slot_98[] = {
	{ "qsExpFile", &static_QUType_QString, 0, QUParameter::In },
	{ "qsResultFile1", &static_QUType_QString, 0, QUParameter::In },
	{ "qsSaveDirName", &static_QUType_QString, 0, QUParameter::In },
	{ "qsTextResultFile", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_98 = {"performIDLTestSeries", 4, param_slot_98 };
    static const QUParameter param_slot_99[] = {
	{ "idlExperiment", &static_QUType_ptr, "ImgDescrList", QUParameter::InOut },
	{ "qsExpPath", &static_QUType_QString, 0, QUParameter::In },
	{ "qsResultFile1", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_99 = {"performIDLTestSeries", 3, param_slot_99 };
    static const QUParameter param_slot_100[] = {
	{ "idlExperiment", &static_QUType_ptr, "ImgDescrList", QUParameter::InOut },
	{ "qsExpPath", &static_QUType_QString, 0, QUParameter::In },
	{ "qsResultFile1", &static_QUType_QString, 0, QUParameter::In },
	{ "qsSaveDirName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_100 = {"performIDLTestSeries", 4, param_slot_100 };
    static const QUParameter param_slot_101[] = {
	{ "idlExperiment", &static_QUType_ptr, "ImgDescrList", QUParameter::InOut },
	{ "qsExpPath", &static_QUType_QString, 0, QUParameter::In },
	{ "qsResultFile1", &static_QUType_QString, 0, QUParameter::In },
	{ "qsSaveDirName", &static_QUType_QString, 0, QUParameter::In },
	{ "qsTextResultFile", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_101 = {"performIDLTestSeries", 5, param_slot_101 };
    static const QUMethod slot_102 = {"processImageSeries", 0, 0 };
    static const QUParameter param_slot_103[] = {
	{ "qsFileName", &static_QUType_QString, 0, QUParameter::In },
	{ "vImgNames", &static_QUType_ptr, "vector<string>", QUParameter::InOut },
	{ "vvAnnots", &static_QUType_ptr, "vector<vector<Hypothesis>>", QUParameter::InOut }
    };
    static const QUMethod slot_103 = {"loadExperimentFile", 3, param_slot_103 };
    static const QUMethod slot_104 = {"listenToSocket", 0, 0 };
    static const QUMethod slot_105 = {"saveImages", 0, 0 };
    static const QUMethod slot_106 = {"saveSegmentationsMatlab", 0, 0 };
    static const QUParameter param_slot_107[] = {
	{ "imgPFig", &static_QUType_ptr, "GrayImage", QUParameter::In },
	{ "imgPGnd", &static_QUType_ptr, "GrayImage", QUParameter::In },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_107 = {"savePFigPGndMatlab", 3, param_slot_107 };
    static const QUParameter param_slot_108[] = {
	{ 0, &static_QUType_QVariant, "\x04", QUParameter::Out }
    };
    static const QUMethod slot_108 = {"getFileList", 1, param_slot_108 };
    static const QUParameter param_slot_109[] = {
	{ 0, &static_QUType_QVariant, "\x04", QUParameter::Out },
	{ "sDir", &static_QUType_ptr, "string", QUParameter::In },
	{ "sCaption", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_109 = {"getFileList", 3, param_slot_109 };
    static const QUParameter param_slot_110[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "fvVector", &static_QUType_ptr, "FeatureVector", QUParameter::InOut }
    };
    static const QUMethod slot_110 = {"readVector", 3, param_slot_110 };
    static const QUParameter param_slot_111[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "mMatrix", &static_QUType_ptr, "vector<FeatureVector>", QUParameter::InOut }
    };
    static const QUMethod slot_111 = {"readMatrix", 3, param_slot_111 };
    static const QUParameter param_slot_112[] = {
	{ "img", &static_QUType_ptr, "OpGrayImage", QUParameter::In },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_112 = {"saveImage", 2, param_slot_112 };
    static const QUParameter param_slot_113[] = {
	{ "img", &static_QUType_ptr, "GrayImage", QUParameter::In },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_113 = {"saveImageAscii", 2, param_slot_113 };
    static const QUParameter param_slot_114[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_114 = {"loadImageAscii", 1, param_slot_114 };
    static const QUParameter param_slot_115[] = {
	{ "ofile", &static_QUType_ptr, "ofstream", QUParameter::InOut },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In }
    };
    static const QUMethod slot_115 = {"writeResultsToDiskUIUC", 3, param_slot_115 };
    static const QUParameter param_slot_116[] = {
	{ "ofile", &static_QUType_ptr, "ofstream", QUParameter::InOut },
	{ "nImgNumber", &static_QUType_int, 0, QUParameter::In },
	{ "vHypos", &static_QUType_ptr, "vector<Hypothesis>", QUParameter::In }
    };
    static const QUMethod slot_116 = {"writeResultsToDiskScore", 3, param_slot_116 };
    static const QUMethod slot_117 = {"loadParams", 0, 0 };
    static const QUParameter param_slot_118[] = {
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_118 = {"loadParams", 1, param_slot_118 };
    static const QUParameter param_slot_119[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_119 = {"loadParams", 1, param_slot_119 };
    static const QUParameter param_slot_120[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_120 = {"loadParams", 2, param_slot_120 };
    static const QUMethod slot_121 = {"saveParams", 0, 0 };
    static const QUParameter param_slot_122[] = {
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_122 = {"saveParams", 1, param_slot_122 };
    static const QUParameter param_slot_123[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_123 = {"saveParams", 1, param_slot_123 };
    static const QUParameter param_slot_124[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_124 = {"saveParams", 2, param_slot_124 };
    static const QUParameter param_slot_125[] = {
	{ "idx", &static_QUType_ptr, "unsigned", QUParameter::In }
    };
    static const QUMethod slot_125 = {"getRecoParams", 1, param_slot_125 };
    static const QMetaData slot_tbl[] = {
	{ "slotSetMapThresh(const QString&)", &slot_0, QMetaData::Public },
	{ "slotSetImgScale(const QString&)", &slot_1, QMetaData::Public },
	{ "slotSetWorldScale(const QString&)", &slot_2, QMetaData::Public },
	{ "slotSetMaxOverlap(const QString&)", &slot_3, QMetaData::Public },
	{ "slotSetMinPFig(const QString&)", &slot_4, QMetaData::Public },
	{ "slotSetWeightPFig(const QString&)", &slot_5, QMetaData::Public },
	{ "slotUpdateImgScale()", &slot_6, QMetaData::Public },
	{ "slotUpdateWorldScale()", &slot_7, QMetaData::Public },
	{ "slotUpdateMapThresh()", &slot_8, QMetaData::Public },
	{ "slotUpdateMaxOverlap()", &slot_9, QMetaData::Public },
	{ "slotUpdateMinPFig()", &slot_10, QMetaData::Public },
	{ "slotUpdateWeightPFig()", &slot_11, QMetaData::Public },
	{ "slotSelectGPFrame(int)", &slot_12, QMetaData::Public },
	{ "slotSetGrayConvOnOff(int)", &slot_13, QMetaData::Public },
	{ "slotSetGammaNormOnOff(int)", &slot_14, QMetaData::Public },
	{ "slotSetUsePatchesOnOff(int)", &slot_15, QMetaData::Public },
	{ "slotSetDrawMapsOnOff(int)", &slot_16, QMetaData::Public },
	{ "slotSetDrawConfOnOff(int)", &slot_17, QMetaData::Public },
	{ "slotSetShowTxtStepsOnOff(int)", &slot_18, QMetaData::Public },
	{ "slotSetShowTxtDetailsOnOff(int)", &slot_19, QMetaData::Public },
	{ "slotSetShowTxtVotingOnOff(int)", &slot_20, QMetaData::Public },
	{ "slotSetShowTxtMDLOnOff(int)", &slot_21, QMetaData::Public },
	{ "slotSetShowTimingsOnOff(int)", &slot_22, QMetaData::Public },
	{ "slotSetShowIntPtsOnOff(int)", &slot_23, QMetaData::Public },
	{ "slotSetFigureOnOff(int)", &slot_24, QMetaData::Public },
	{ "slotSetDrawTightBBOnOff(int)", &slot_25, QMetaData::Public },
	{ "slotSetEvalUniqueOnOff(int)", &slot_26, QMetaData::Public },
	{ "slotSetDisplayVSOnOff(int)", &slot_27, QMetaData::Public },
	{ "slotSetDisplaySupportOnOff(int)", &slot_28, QMetaData::Public },
	{ "slotSetDisplaySegmentOnOff(int)", &slot_29, QMetaData::Public },
	{ "slotSetDisplayMultiCueOnOff(int)", &slot_30, QMetaData::Public },
	{ "slotSetWriteResultsOnOff(int)", &slot_31, QMetaData::Public },
	{ "slotSetWriteSegsOnOff(int)", &slot_32, QMetaData::Public },
	{ "slotSetDoMDLOnOff(int)", &slot_33, QMetaData::Public },
	{ "slotSetRejectOverlapOnOff(int)", &slot_34, QMetaData::Public },
	{ "slotSetRejectPFigOnOff(int)", &slot_35, QMetaData::Public },
	{ "setShowGUI(bool)", &slot_36, QMetaData::Public },
	{ "loadImage()", &slot_37, QMetaData::Public },
	{ "loadImage(QString)", &slot_38, QMetaData::Public },
	{ "drawGroundPlane()", &slot_39, QMetaData::Public },
	{ "drawDistHLine(float,float,float,float)", &slot_40, QMetaData::Public },
	{ "drawDistHLine(float,float,float,float,QColor)", &slot_41, QMetaData::Public },
	{ "drawDistVLine(float,float,float,float)", &slot_42, QMetaData::Public },
	{ "drawDistVLine(float,float,float,float,QColor)", &slot_43, QMetaData::Public },
	{ "drawDistZLine(float,float,float,float)", &slot_44, QMetaData::Public },
	{ "drawDistZLine(float,float,float,float,QColor)", &slot_45, QMetaData::Public },
	{ "showResultImg(int,int)", &slot_46, QMetaData::Public },
	{ "showResultImg()", &slot_47, QMetaData::Public },
	{ "addDetector()", &slot_48, QMetaData::Public },
	{ "addDetector(QString)", &slot_49, QMetaData::Public },
	{ "addDetector(QString,bool)", &slot_50, QMetaData::Public },
	{ "detectorTableClicked(int,int,int,const QPoint&)", &slot_51, QMetaData::Public },
	{ "showDetectorWindow(int)", &slot_52, QMetaData::Public },
	{ "addDetectorTableEntry(string,string,string,string,string)", &slot_53, QMetaData::Public },
	{ "updateDetectorTable()", &slot_54, QMetaData::Public },
	{ "cueTableClicked(int,int,int,const QPoint&)", &slot_55, QMetaData::Public },
	{ "showCueWindow(int)", &slot_56, QMetaData::Public },
	{ "addCueTableEntry(string,string,string,string,string)", &slot_57, QMetaData::Public },
	{ "updateCueTableEntry(unsigned,string,string,string,string,string)", &slot_58, QMetaData::Public },
	{ "collectCueInformation()", &slot_59, QMetaData::Public },
	{ "processImage(QString)", &slot_60, QMetaData::Public },
	{ "collectPatches()", &slot_61, QMetaData::Public },
	{ "collectPatches(bool)", &slot_62, QMetaData::Public },
	{ "drawInterestPoints()", &slot_63, QMetaData::Public },
	{ "drawInterestPointsEllipse(float)", &slot_64, QMetaData::Public },
	{ "displayPatchesForBrowsing(vector<OpGrayImage>&)", &slot_65, QMetaData::Public },
	{ "displayPatchesForBrowsing(vector<OpGrayImage>&,int)", &slot_66, QMetaData::Public },
	{ "displayPatchesForBrowsing(vector<OpGrayImage>&,int,int)", &slot_67, QMetaData::Public },
	{ "compareFeatures()", &slot_68, QMetaData::Public },
	{ "processTestImg(QString,int,vector<Hypothesis>&,vector<Hypothesis>&,vector<OpGrayImage>&,vector<OpGrayImage>&,vector<OpGrayImage>&,bool)", &slot_69, QMetaData::Public },
	{ "processTestImgBothDir(QString,int,vector<Hypothesis>&,vector<Hypothesis>&,vector<OpGrayImage>&,vector<OpGrayImage>&,vector<OpGrayImage>&,bool)", &slot_70, QMetaData::Public },
	{ "processTestImgStd()", &slot_71, QMetaData::Public },
	{ "processTestImgStd(QString)", &slot_72, QMetaData::Public },
	{ "processTestImgStd(QString,int,vector<Hypothesis>&,vector<OpGrayImage>&,vector<OpGrayImage>&,vector<OpGrayImage>&,ofstream&)", &slot_73, QMetaData::Public },
	{ "processTestImgStd(QString,int,vector<Hypothesis>&,vector<OpGrayImage>&,vector<OpGrayImage>&,vector<OpGrayImage>&,ofstream&,bool)", &slot_74, QMetaData::Public },
	{ "processTestImgIDL(QString,int,QString,vector<Hypothesis>&,ImgDescr&,ImgDescr&)", &slot_75, QMetaData::Public },
	{ "processTestImgIDL(QString,int,QString,vector<Hypothesis>&,ImgDescr&,ImgDescr&,bool)", &slot_76, QMetaData::Public },
	{ "displayRecoResults(const vector<Hypothesis>&,const vector<OpGrayImage>&,const vector<OpGrayImage>&,const vector<OpGrayImage>&)", &slot_77, QMetaData::Public },
	{ "displayRecoResults(const vector<Hypothesis>&,const vector<OpGrayImage>&,const vector<OpGrayImage>&,const vector<OpGrayImage>&,bool)", &slot_78, QMetaData::Public },
	{ "computeTightBBox(OpGrayImage)", &slot_79, QMetaData::Public },
	{ "computeTightBBox(OpGrayImage,int)", &slot_80, QMetaData::Public },
	{ "computeTightBBox(OpGrayImage,int,int)", &slot_81, QMetaData::Public },
	{ "countPatches(Hypothesis&,unsigned,const vector<HoughVote>&)", &slot_82, QMetaData::Public },
	{ "resampleHypothesis(Hypothesis&,Segmentation&)", &slot_83, QMetaData::Public },
	{ "loadSilhouettes()", &slot_84, QMetaData::Public },
	{ "verifyHyposTemplate(const vector<Hypothesis>&,const vector<Hypothesis>&,const vector<OpGrayImage>&,const vector<OpGrayImage>&,const vector<OpGrayImage>&,vector<Hypothesis>&,vector<Hypothesis>&,vector<EdgePtVec>&,vector<int>&,vector<QImage>&)", &slot_85, QMetaData::Public },
	{ "compBhattaMask(const OpGrayImage&,const OpGrayImage&,int,int)", &slot_86, QMetaData::Public },
	{ "drawHypothesis(const Hypothesis&,const QColor&,bool)", &slot_87, QMetaData::Public },
	{ "refreshSrcImg()", &slot_88, QMetaData::Public },
	{ "displayResultImg(const QImage&)", &slot_89, QMetaData::Public },
	{ "displayResultImg(const QImage&,OpGrayImage)", &slot_90, QMetaData::Public },
	{ "displayScaleFootprint()", &slot_91, QMetaData::Public },
	{ "transformPoint(InterestPoint&)", &slot_92, QMetaData::Public },
	{ "transformPoint(int,int)", &slot_93, QMetaData::Public },
	{ "computeBoundingBoxOverlap(Hypothesis,Hypothesis)", &slot_94, QMetaData::Public },
	{ "performIDLTestSeries()", &slot_95, QMetaData::Public },
	{ "performIDLTestSeries(QString,QString)", &slot_96, QMetaData::Public },
	{ "performIDLTestSeries(QString,QString,QString)", &slot_97, QMetaData::Public },
	{ "performIDLTestSeries(QString,QString,QString,QString)", &slot_98, QMetaData::Public },
	{ "performIDLTestSeries(ImgDescrList&,QString,QString)", &slot_99, QMetaData::Public },
	{ "performIDLTestSeries(ImgDescrList&,QString,QString,QString)", &slot_100, QMetaData::Public },
	{ "performIDLTestSeries(ImgDescrList&,QString,QString,QString,QString)", &slot_101, QMetaData::Public },
	{ "processImageSeries()", &slot_102, QMetaData::Public },
	{ "loadExperimentFile(QString,vector<string>&,vector<vector<Hypothesis>>&)", &slot_103, QMetaData::Public },
	{ "listenToSocket()", &slot_104, QMetaData::Public },
	{ "saveImages()", &slot_105, QMetaData::Public },
	{ "saveSegmentationsMatlab()", &slot_106, QMetaData::Public },
	{ "savePFigPGndMatlab(GrayImage,GrayImage,string)", &slot_107, QMetaData::Public },
	{ "getFileList()", &slot_108, QMetaData::Public },
	{ "getFileList(string,string)", &slot_109, QMetaData::Public },
	{ "readVector(string,FeatureVector&)", &slot_110, QMetaData::Public },
	{ "readMatrix(string,vector<FeatureVector>&)", &slot_111, QMetaData::Public },
	{ "saveImage(OpGrayImage,string)", &slot_112, QMetaData::Public },
	{ "saveImageAscii(GrayImage,string)", &slot_113, QMetaData::Public },
	{ "loadImageAscii(string)", &slot_114, QMetaData::Public },
	{ "writeResultsToDiskUIUC(ofstream&,int,vector<Hypothesis>)", &slot_115, QMetaData::Public },
	{ "writeResultsToDiskScore(ofstream&,int,vector<Hypothesis>)", &slot_116, QMetaData::Public },
	{ "loadParams()", &slot_117, QMetaData::Public },
	{ "loadParams(bool)", &slot_118, QMetaData::Public },
	{ "loadParams(string)", &slot_119, QMetaData::Public },
	{ "loadParams(string,bool)", &slot_120, QMetaData::Public },
	{ "saveParams()", &slot_121, QMetaData::Public },
	{ "saveParams(bool)", &slot_122, QMetaData::Public },
	{ "saveParams(string)", &slot_123, QMetaData::Public },
	{ "saveParams(string,bool)", &slot_124, QMetaData::Public },
	{ "getRecoParams(unsigned)", &slot_125, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigMapThreshChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"sigImgScaleChanged", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"sigWorldScaleChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"sigAgreeingChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"sigFromChanged", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"sigToChanged", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_6 = {"sigMaxOverlapChanged", 1, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"sigMinPFigChanged", 1, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_8 = {"sigWeightPFigChanged", 1, param_signal_8 };
    static const QUParameter param_signal_9[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_9 = {"sigMinPFigRefinedChanged", 1, param_signal_9 };
    static const QMetaData signal_tbl[] = {
	{ "sigMapThreshChanged(const QString&)", &signal_0, QMetaData::Public },
	{ "sigImgScaleChanged(const QString&)", &signal_1, QMetaData::Public },
	{ "sigWorldScaleChanged(const QString&)", &signal_2, QMetaData::Public },
	{ "sigAgreeingChanged(const QString&)", &signal_3, QMetaData::Public },
	{ "sigFromChanged(const QString&)", &signal_4, QMetaData::Public },
	{ "sigToChanged(const QString&)", &signal_5, QMetaData::Public },
	{ "sigMaxOverlapChanged(const QString&)", &signal_6, QMetaData::Public },
	{ "sigMinPFigChanged(const QString&)", &signal_7, QMetaData::Public },
	{ "sigWeightPFigChanged(const QString&)", &signal_8, QMetaData::Public },
	{ "sigMinPFigRefinedChanged(const QString&)", &signal_9, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ISMReco", parentObject,
	slot_tbl, 126,
	signal_tbl, 10,
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

// SIGNAL sigImgScaleChanged
void ISMReco::sigImgScaleChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL sigWorldScaleChanged
void ISMReco::sigWorldScaleChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL sigAgreeingChanged
void ISMReco::sigAgreeingChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL sigFromChanged
void ISMReco::sigFromChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL sigToChanged
void ISMReco::sigToChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL sigMaxOverlapChanged
void ISMReco::sigMaxOverlapChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 6, t0 );
}

// SIGNAL sigMinPFigChanged
void ISMReco::sigMinPFigChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 7, t0 );
}

// SIGNAL sigWeightPFigChanged
void ISMReco::sigWeightPFigChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 8, t0 );
}

// SIGNAL sigMinPFigRefinedChanged
void ISMReco::sigMinPFigRefinedChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 9, t0 );
}

bool ISMReco::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotSetMapThresh((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: slotSetImgScale((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: slotSetWorldScale((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: slotSetMaxOverlap((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: slotSetMinPFig((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: slotSetWeightPFig((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: slotUpdateImgScale(); break;
    case 7: slotUpdateWorldScale(); break;
    case 8: slotUpdateMapThresh(); break;
    case 9: slotUpdateMaxOverlap(); break;
    case 10: slotUpdateMinPFig(); break;
    case 11: slotUpdateWeightPFig(); break;
    case 12: slotSelectGPFrame((int)static_QUType_int.get(_o+1)); break;
    case 13: slotSetGrayConvOnOff((int)static_QUType_int.get(_o+1)); break;
    case 14: slotSetGammaNormOnOff((int)static_QUType_int.get(_o+1)); break;
    case 15: slotSetUsePatchesOnOff((int)static_QUType_int.get(_o+1)); break;
    case 16: slotSetDrawMapsOnOff((int)static_QUType_int.get(_o+1)); break;
    case 17: slotSetDrawConfOnOff((int)static_QUType_int.get(_o+1)); break;
    case 18: slotSetShowTxtStepsOnOff((int)static_QUType_int.get(_o+1)); break;
    case 19: slotSetShowTxtDetailsOnOff((int)static_QUType_int.get(_o+1)); break;
    case 20: slotSetShowTxtVotingOnOff((int)static_QUType_int.get(_o+1)); break;
    case 21: slotSetShowTxtMDLOnOff((int)static_QUType_int.get(_o+1)); break;
    case 22: slotSetShowTimingsOnOff((int)static_QUType_int.get(_o+1)); break;
    case 23: slotSetShowIntPtsOnOff((int)static_QUType_int.get(_o+1)); break;
    case 24: slotSetFigureOnOff((int)static_QUType_int.get(_o+1)); break;
    case 25: slotSetDrawTightBBOnOff((int)static_QUType_int.get(_o+1)); break;
    case 26: slotSetEvalUniqueOnOff((int)static_QUType_int.get(_o+1)); break;
    case 27: slotSetDisplayVSOnOff((int)static_QUType_int.get(_o+1)); break;
    case 28: slotSetDisplaySupportOnOff((int)static_QUType_int.get(_o+1)); break;
    case 29: slotSetDisplaySegmentOnOff((int)static_QUType_int.get(_o+1)); break;
    case 30: slotSetDisplayMultiCueOnOff((int)static_QUType_int.get(_o+1)); break;
    case 31: slotSetWriteResultsOnOff((int)static_QUType_int.get(_o+1)); break;
    case 32: slotSetWriteSegsOnOff((int)static_QUType_int.get(_o+1)); break;
    case 33: slotSetDoMDLOnOff((int)static_QUType_int.get(_o+1)); break;
    case 34: slotSetRejectOverlapOnOff((int)static_QUType_int.get(_o+1)); break;
    case 35: slotSetRejectPFigOnOff((int)static_QUType_int.get(_o+1)); break;
    case 36: setShowGUI((bool)static_QUType_bool.get(_o+1)); break;
    case 37: loadImage(); break;
    case 38: loadImage((QString)static_QUType_QString.get(_o+1)); break;
    case 39: drawGroundPlane(); break;
    case 40: drawDistHLine((float)(*((float*)static_QUType_ptr.get(_o+1))),(float)(*((float*)static_QUType_ptr.get(_o+2))),(float)(*((float*)static_QUType_ptr.get(_o+3))),(float)(*((float*)static_QUType_ptr.get(_o+4)))); break;
    case 41: drawDistHLine((float)(*((float*)static_QUType_ptr.get(_o+1))),(float)(*((float*)static_QUType_ptr.get(_o+2))),(float)(*((float*)static_QUType_ptr.get(_o+3))),(float)(*((float*)static_QUType_ptr.get(_o+4))),(QColor)(*((QColor*)static_QUType_ptr.get(_o+5)))); break;
    case 42: drawDistVLine((float)(*((float*)static_QUType_ptr.get(_o+1))),(float)(*((float*)static_QUType_ptr.get(_o+2))),(float)(*((float*)static_QUType_ptr.get(_o+3))),(float)(*((float*)static_QUType_ptr.get(_o+4)))); break;
    case 43: drawDistVLine((float)(*((float*)static_QUType_ptr.get(_o+1))),(float)(*((float*)static_QUType_ptr.get(_o+2))),(float)(*((float*)static_QUType_ptr.get(_o+3))),(float)(*((float*)static_QUType_ptr.get(_o+4))),(QColor)(*((QColor*)static_QUType_ptr.get(_o+5)))); break;
    case 44: drawDistZLine((float)(*((float*)static_QUType_ptr.get(_o+1))),(float)(*((float*)static_QUType_ptr.get(_o+2))),(float)(*((float*)static_QUType_ptr.get(_o+3))),(float)(*((float*)static_QUType_ptr.get(_o+4)))); break;
    case 45: drawDistZLine((float)(*((float*)static_QUType_ptr.get(_o+1))),(float)(*((float*)static_QUType_ptr.get(_o+2))),(float)(*((float*)static_QUType_ptr.get(_o+3))),(float)(*((float*)static_QUType_ptr.get(_o+4))),(QColor)(*((QColor*)static_QUType_ptr.get(_o+5)))); break;
    case 46: showResultImg((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 47: showResultImg(); break;
    case 48: addDetector(); break;
    case 49: addDetector((QString)static_QUType_QString.get(_o+1)); break;
    case 50: addDetector((QString)static_QUType_QString.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    case 51: detectorTableClicked((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 52: showDetectorWindow((int)static_QUType_int.get(_o+1)); break;
    case 53: addDetectorTableEntry((string)(*((string*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(string)(*((string*)static_QUType_ptr.get(_o+3))),(string)(*((string*)static_QUType_ptr.get(_o+4))),(string)(*((string*)static_QUType_ptr.get(_o+5)))); break;
    case 54: updateDetectorTable(); break;
    case 55: cueTableClicked((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 56: showCueWindow((int)static_QUType_int.get(_o+1)); break;
    case 57: addCueTableEntry((string)(*((string*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(string)(*((string*)static_QUType_ptr.get(_o+3))),(string)(*((string*)static_QUType_ptr.get(_o+4))),(string)(*((string*)static_QUType_ptr.get(_o+5)))); break;
    case 58: updateCueTableEntry((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2))),(string)(*((string*)static_QUType_ptr.get(_o+3))),(string)(*((string*)static_QUType_ptr.get(_o+4))),(string)(*((string*)static_QUType_ptr.get(_o+5))),(string)(*((string*)static_QUType_ptr.get(_o+6)))); break;
    case 59: collectCueInformation(); break;
    case 60: processImage((QString)static_QUType_QString.get(_o+1)); break;
    case 61: collectPatches(); break;
    case 62: collectPatches((bool)static_QUType_bool.get(_o+1)); break;
    case 63: drawInterestPoints(); break;
    case 64: drawInterestPointsEllipse((float)(*((float*)static_QUType_ptr.get(_o+1)))); break;
    case 65: displayPatchesForBrowsing((vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+1))); break;
    case 66: displayPatchesForBrowsing((vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2)); break;
    case 67: displayPatchesForBrowsing((vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3)); break;
    case 68: compareFeatures(); break;
    case 69: processTestImg((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+4)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+5)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+6)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+7)),(bool)static_QUType_bool.get(_o+8)); break;
    case 70: processTestImgBothDir((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+4)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+5)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+6)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+7)),(bool)static_QUType_bool.get(_o+8)); break;
    case 71: processTestImgStd(); break;
    case 72: processTestImgStd((QString)static_QUType_QString.get(_o+1)); break;
    case 73: processTestImgStd((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+4)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+5)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+6)),(ofstream&)*((ofstream*)static_QUType_ptr.get(_o+7))); break;
    case 74: processTestImgStd((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+4)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+5)),(vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+6)),(ofstream&)*((ofstream*)static_QUType_ptr.get(_o+7)),(bool)static_QUType_bool.get(_o+8)); break;
    case 75: processTestImgIDL((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(QString)static_QUType_QString.get(_o+3),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+4)),(ImgDescr&)*((ImgDescr*)static_QUType_ptr.get(_o+5)),(ImgDescr&)*((ImgDescr*)static_QUType_ptr.get(_o+6))); break;
    case 76: processTestImgIDL((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2),(QString)static_QUType_QString.get(_o+3),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+4)),(ImgDescr&)*((ImgDescr*)static_QUType_ptr.get(_o+5)),(ImgDescr&)*((ImgDescr*)static_QUType_ptr.get(_o+6)),(bool)static_QUType_bool.get(_o+7)); break;
    case 77: displayRecoResults((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+2)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+3)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+4))); break;
    case 78: displayRecoResults((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+2)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+3)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+4)),(bool)static_QUType_bool.get(_o+5)); break;
    case 79: computeTightBBox((OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+1)))); break;
    case 80: computeTightBBox((OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+1))),(int)static_QUType_int.get(_o+2)); break;
    case 81: computeTightBBox((OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+1))),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3)); break;
    case 82: countPatches((Hypothesis&)*((Hypothesis*)static_QUType_ptr.get(_o+1)),(unsigned)(*((unsigned*)static_QUType_ptr.get(_o+2))),(const vector<HoughVote>&)*((const vector<HoughVote>*)static_QUType_ptr.get(_o+3))); break;
    case 83: resampleHypothesis((Hypothesis&)*((Hypothesis*)static_QUType_ptr.get(_o+1)),(Segmentation&)*((Segmentation*)static_QUType_ptr.get(_o+2))); break;
    case 84: loadSilhouettes(); break;
    case 85: verifyHyposTemplate((const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+1)),(const vector<Hypothesis>&)*((const vector<Hypothesis>*)static_QUType_ptr.get(_o+2)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+3)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+4)),(const vector<OpGrayImage>&)*((const vector<OpGrayImage>*)static_QUType_ptr.get(_o+5)),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+6)),(vector<Hypothesis>&)*((vector<Hypothesis>*)static_QUType_ptr.get(_o+7)),(vector<EdgePtVec>&)*((vector<EdgePtVec>*)static_QUType_ptr.get(_o+8)),(vector<int>&)*((vector<int>*)static_QUType_ptr.get(_o+9)),(vector<QImage>&)*((vector<QImage>*)static_QUType_ptr.get(_o+10))); break;
    case 86: static_QUType_double.set(_o,compBhattaMask((const OpGrayImage&)*((const OpGrayImage*)static_QUType_ptr.get(_o+1)),(const OpGrayImage&)*((const OpGrayImage*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3),(int)static_QUType_int.get(_o+4))); break;
    case 87: drawHypothesis((const Hypothesis&)*((const Hypothesis*)static_QUType_ptr.get(_o+1)),(const QColor&)*((const QColor*)static_QUType_ptr.get(_o+2)),(bool)static_QUType_bool.get(_o+3)); break;
    case 88: refreshSrcImg(); break;
    case 89: displayResultImg((const QImage&)*((const QImage*)static_QUType_ptr.get(_o+1))); break;
    case 90: displayResultImg((const QImage&)*((const QImage*)static_QUType_ptr.get(_o+1)),(OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+2)))); break;
    case 91: displayScaleFootprint(); break;
    case 92: static_QUType_int.set(_o,transformPoint((InterestPoint&)*((InterestPoint*)static_QUType_ptr.get(_o+1)))); break;
    case 93: static_QUType_int.set(_o,transformPoint((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2))); break;
    case 94: computeBoundingBoxOverlap((Hypothesis)(*((Hypothesis*)static_QUType_ptr.get(_o+1))),(Hypothesis)(*((Hypothesis*)static_QUType_ptr.get(_o+2)))); break;
    case 95: performIDLTestSeries(); break;
    case 96: performIDLTestSeries((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 97: performIDLTestSeries((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3)); break;
    case 98: performIDLTestSeries((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3),(QString)static_QUType_QString.get(_o+4)); break;
    case 99: performIDLTestSeries((ImgDescrList&)*((ImgDescrList*)static_QUType_ptr.get(_o+1)),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3)); break;
    case 100: performIDLTestSeries((ImgDescrList&)*((ImgDescrList*)static_QUType_ptr.get(_o+1)),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3),(QString)static_QUType_QString.get(_o+4)); break;
    case 101: performIDLTestSeries((ImgDescrList&)*((ImgDescrList*)static_QUType_ptr.get(_o+1)),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3),(QString)static_QUType_QString.get(_o+4),(QString)static_QUType_QString.get(_o+5)); break;
    case 102: processImageSeries(); break;
    case 103: loadExperimentFile((QString)static_QUType_QString.get(_o+1),(vector<string>&)*((vector<string>*)static_QUType_ptr.get(_o+2)),(vector<vector<Hypothesis> >&)*((vector<vector<Hypothesis> >*)static_QUType_ptr.get(_o+3))); break;
    case 104: listenToSocket(); break;
    case 105: saveImages(); break;
    case 106: saveSegmentationsMatlab(); break;
    case 107: savePFigPGndMatlab((GrayImage)(*((GrayImage*)static_QUType_ptr.get(_o+1))),(GrayImage)(*((GrayImage*)static_QUType_ptr.get(_o+2))),(string)(*((string*)static_QUType_ptr.get(_o+3)))); break;
    case 108: static_QUType_QVariant.set(_o,QVariant(getFileList())); break;
    case 109: static_QUType_QVariant.set(_o,QVariant(getFileList((string)(*((string*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2)))))); break;
    case 110: static_QUType_bool.set(_o,readVector((string)(*((string*)static_QUType_ptr.get(_o+1))),(FeatureVector&)*((FeatureVector*)static_QUType_ptr.get(_o+2)))); break;
    case 111: static_QUType_bool.set(_o,readMatrix((string)(*((string*)static_QUType_ptr.get(_o+1))),(vector<FeatureVector>&)*((vector<FeatureVector>*)static_QUType_ptr.get(_o+2)))); break;
    case 112: saveImage((OpGrayImage)(*((OpGrayImage*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2)))); break;
    case 113: saveImageAscii((GrayImage)(*((GrayImage*)static_QUType_ptr.get(_o+1))),(string)(*((string*)static_QUType_ptr.get(_o+2)))); break;
    case 114: loadImageAscii((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 115: writeResultsToDiskUIUC((ofstream&)*((ofstream*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>)(*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)))); break;
    case 116: writeResultsToDiskScore((ofstream&)*((ofstream*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2),(vector<Hypothesis>)(*((vector<Hypothesis>*)static_QUType_ptr.get(_o+3)))); break;
    case 117: loadParams(); break;
    case 118: loadParams((bool)static_QUType_bool.get(_o+1)); break;
    case 119: loadParams((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 120: loadParams((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 121: saveParams(); break;
    case 122: saveParams((bool)static_QUType_bool.get(_o+1)); break;
    case 123: saveParams((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 124: saveParams((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 125: getRecoParams((unsigned)(*((unsigned*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ISMReco::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigMapThreshChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: sigImgScaleChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: sigWorldScaleChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: sigAgreeingChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: sigFromChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: sigToChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: sigMaxOverlapChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 7: sigMinPFigChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: sigWeightPFigChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 9: sigMinPFigRefinedChanged((const QString&)static_QUType_QString.get(_o+1)); break;
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
