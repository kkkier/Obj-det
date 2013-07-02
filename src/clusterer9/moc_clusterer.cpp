/****************************************************************************
** Clusterer meta object code from reading C++ file 'clusterer.hh'
**
** Created: Wed Jan 30 11:39:36 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "clusterer.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Clusterer::className() const
{
    return "Clusterer";
}

QMetaObject *Clusterer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Clusterer( "Clusterer", &Clusterer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Clusterer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Clusterer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Clusterer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Clusterer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Clusterer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotSetObjWidth", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotSetObjHeight", 1, param_slot_1 };
    static const QUMethod slot_2 = {"slotUpdateObjWidth", 0, 0 };
    static const QUMethod slot_3 = {"slotUpdateObjHeight", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotSelectFixObjDim", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"slotSetUsePatchesOnOff", 1, param_slot_5 };
    static const QUMethod slot_6 = {"loadImage", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ "name", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"loadImage", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "qsName", &static_QUType_QString, 0, QUParameter::In },
	{ "rBBox", &static_QUType_ptr, "Rect", QUParameter::In }
    };
    static const QUMethod slot_8 = {"loadImageBBox", 2, param_slot_8 };
    static const QUMethod slot_9 = {"loadImageSet", 0, 0 };
    static const QUMethod slot_10 = {"loadIDLImageSet", 0, 0 };
    static const QUParameter param_slot_11[] = {
	{ "qsIdlFile", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"loadIDLImageSet", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "qstr", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"processImage", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "qstr", &static_QUType_QString, 0, QUParameter::In },
	{ "rBBox", &static_QUType_ptr, "Rect", QUParameter::In }
    };
    static const QUMethod slot_13 = {"processImageBBox", 2, param_slot_13 };
    static const QUMethod slot_14 = {"collectPatches", 0, 0 };
    static const QUParameter param_slot_15[] = {
	{ "process", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"collectPatches", 1, param_slot_15 };
    static const QUMethod slot_16 = {"drawInterestPoints", 0, 0 };
    static const QUMethod slot_17 = {"drawInterestPointsEllipse", 0, 0 };
    static const QUParameter param_slot_18[] = {
	{ "vPatches", &static_QUType_ptr, "vector<OpGrayImage>", QUParameter::InOut }
    };
    static const QUMethod slot_18 = {"displayPatchesForBrowsing", 1, param_slot_18 };
    static const QUMethod slot_19 = {"clusterPatches", 0, 0 };
    static const QUMethod slot_20 = {"remove1PatchClusters", 0, 0 };
    static const QUMethod slot_21 = {"removeClutteredPatches", 0, 0 };
    static const QUMethod slot_22 = {"recreateCodebookFromTrace", 0, 0 };
    static const QUMethod slot_23 = {"loadCodebook", 0, 0 };
    static const QUParameter param_slot_24[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_24 = {"loadCodebook", 1, param_slot_24 };
    static const QUMethod slot_25 = {"appendCodebook", 0, 0 };
    static const QUParameter param_slot_26[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_26 = {"appendCodebook", 1, param_slot_26 };
    static const QUMethod slot_27 = {"saveCodebook", 0, 0 };
    static const QUParameter param_slot_28[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_28 = {"saveCodebook", 1, param_slot_28 };
    static const QUMethod slot_29 = {"computeFVEigenspace", 0, 0 };
    static const QUMethod slot_30 = {"loadESMatlab", 0, 0 };
    static const QUParameter param_slot_31[] = {
	{ "vEigenVectors", &static_QUType_ptr, "vector<FeatureVector>", QUParameter::In },
	{ "fvEigenValues", &static_QUType_ptr, "FeatureVector", QUParameter::In },
	{ "fvAvgImage", &static_QUType_ptr, "FeatureVector", QUParameter::In }
    };
    static const QUMethod slot_31 = {"displayEigenspace", 3, param_slot_31 };
    static const QUMethod slot_32 = {"savePatchesMatlab", 0, 0 };
    static const QUMethod slot_33 = {"saveFeaturesMatlab", 0, 0 };
    static const QUMethod slot_34 = {"saveClustersMatlab", 0, 0 };
    static const QUMethod slot_35 = {"saveFeatureLocations", 0, 0 };
    static const QUParameter param_slot_36[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_36 = {"saveFeatureLocations", 1, param_slot_36 };
    static const QUMethod slot_37 = {"saveFeatureClasses", 0, 0 };
    static const QUParameter param_slot_38[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_38 = {"saveFeatureClasses", 1, param_slot_38 };
    static const QUParameter param_slot_39[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "vFeatureClass", &static_QUType_ptr, "vector<int>", QUParameter::InOut }
    };
    static const QUMethod slot_39 = {"loadFeatureClasses", 2, param_slot_39 };
    static const QUMethod slot_40 = {"saveClusterTrace", 0, 0 };
    static const QUMethod slot_41 = {"saveClusteredImages", 0, 0 };
    static const QUParameter param_slot_42[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "fvVector", &static_QUType_ptr, "FeatureVector", QUParameter::InOut }
    };
    static const QUMethod slot_42 = {"readVector", 3, param_slot_42 };
    static const QUParameter param_slot_43[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "mMatrix", &static_QUType_ptr, "vector<FeatureVector>", QUParameter::InOut }
    };
    static const QUMethod slot_43 = {"readMatrix", 3, param_slot_43 };
    static const QUMethod slot_44 = {"loadParams", 0, 0 };
    static const QUParameter param_slot_45[] = {
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_45 = {"loadParams", 1, param_slot_45 };
    static const QUParameter param_slot_46[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_46 = {"loadParams", 1, param_slot_46 };
    static const QUParameter param_slot_47[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_47 = {"loadParams", 2, param_slot_47 };
    static const QUMethod slot_48 = {"saveParams", 0, 0 };
    static const QUParameter param_slot_49[] = {
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_49 = {"saveParams", 1, param_slot_49 };
    static const QUParameter param_slot_50[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In }
    };
    static const QUMethod slot_50 = {"saveParams", 1, param_slot_50 };
    static const QUParameter param_slot_51[] = {
	{ "sFileName", &static_QUType_ptr, "string", QUParameter::In },
	{ "bVerbose", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_51 = {"saveParams", 2, param_slot_51 };
    static const QMetaData slot_tbl[] = {
	{ "slotSetObjWidth(const QString&)", &slot_0, QMetaData::Public },
	{ "slotSetObjHeight(const QString&)", &slot_1, QMetaData::Public },
	{ "slotUpdateObjWidth()", &slot_2, QMetaData::Public },
	{ "slotUpdateObjHeight()", &slot_3, QMetaData::Public },
	{ "slotSelectFixObjDim(int)", &slot_4, QMetaData::Public },
	{ "slotSetUsePatchesOnOff(int)", &slot_5, QMetaData::Public },
	{ "loadImage()", &slot_6, QMetaData::Public },
	{ "loadImage(QString)", &slot_7, QMetaData::Public },
	{ "loadImageBBox(QString,const Rect&)", &slot_8, QMetaData::Public },
	{ "loadImageSet()", &slot_9, QMetaData::Public },
	{ "loadIDLImageSet()", &slot_10, QMetaData::Public },
	{ "loadIDLImageSet(const QString&)", &slot_11, QMetaData::Public },
	{ "processImage(QString)", &slot_12, QMetaData::Public },
	{ "processImageBBox(QString,const Rect&)", &slot_13, QMetaData::Public },
	{ "collectPatches()", &slot_14, QMetaData::Public },
	{ "collectPatches(bool)", &slot_15, QMetaData::Public },
	{ "drawInterestPoints()", &slot_16, QMetaData::Public },
	{ "drawInterestPointsEllipse()", &slot_17, QMetaData::Public },
	{ "displayPatchesForBrowsing(vector<OpGrayImage>&)", &slot_18, QMetaData::Public },
	{ "clusterPatches()", &slot_19, QMetaData::Public },
	{ "remove1PatchClusters()", &slot_20, QMetaData::Public },
	{ "removeClutteredPatches()", &slot_21, QMetaData::Public },
	{ "recreateCodebookFromTrace()", &slot_22, QMetaData::Public },
	{ "loadCodebook()", &slot_23, QMetaData::Public },
	{ "loadCodebook(string)", &slot_24, QMetaData::Public },
	{ "appendCodebook()", &slot_25, QMetaData::Public },
	{ "appendCodebook(string)", &slot_26, QMetaData::Public },
	{ "saveCodebook()", &slot_27, QMetaData::Public },
	{ "saveCodebook(string)", &slot_28, QMetaData::Public },
	{ "computeFVEigenspace()", &slot_29, QMetaData::Public },
	{ "loadESMatlab()", &slot_30, QMetaData::Public },
	{ "displayEigenspace(const vector<FeatureVector>&,const FeatureVector&,const FeatureVector&)", &slot_31, QMetaData::Public },
	{ "savePatchesMatlab()", &slot_32, QMetaData::Public },
	{ "saveFeaturesMatlab()", &slot_33, QMetaData::Public },
	{ "saveClustersMatlab()", &slot_34, QMetaData::Public },
	{ "saveFeatureLocations()", &slot_35, QMetaData::Public },
	{ "saveFeatureLocations(string)", &slot_36, QMetaData::Public },
	{ "saveFeatureClasses()", &slot_37, QMetaData::Public },
	{ "saveFeatureClasses(string)", &slot_38, QMetaData::Public },
	{ "loadFeatureClasses(string,vector<int>&)", &slot_39, QMetaData::Public },
	{ "saveClusterTrace()", &slot_40, QMetaData::Public },
	{ "saveClusteredImages()", &slot_41, QMetaData::Public },
	{ "readVector(string,FeatureVector&)", &slot_42, QMetaData::Public },
	{ "readMatrix(string,vector<FeatureVector>&)", &slot_43, QMetaData::Public },
	{ "loadParams()", &slot_44, QMetaData::Public },
	{ "loadParams(bool)", &slot_45, QMetaData::Public },
	{ "loadParams(string)", &slot_46, QMetaData::Public },
	{ "loadParams(string,bool)", &slot_47, QMetaData::Public },
	{ "saveParams()", &slot_48, QMetaData::Public },
	{ "saveParams(bool)", &slot_49, QMetaData::Public },
	{ "saveParams(string)", &slot_50, QMetaData::Public },
	{ "saveParams(string,bool)", &slot_51, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigObjWidthChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"sigObjHeightChanged", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "sigObjWidthChanged(const QString&)", &signal_0, QMetaData::Public },
	{ "sigObjHeightChanged(const QString&)", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Clusterer", parentObject,
	slot_tbl, 52,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Clusterer.setMetaObject( metaObj );
    return metaObj;
}

void* Clusterer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Clusterer" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sigObjWidthChanged
void Clusterer::sigObjWidthChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL sigObjHeightChanged
void Clusterer::sigObjHeightChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

bool Clusterer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotSetObjWidth((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: slotSetObjHeight((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: slotUpdateObjWidth(); break;
    case 3: slotUpdateObjHeight(); break;
    case 4: slotSelectFixObjDim((int)static_QUType_int.get(_o+1)); break;
    case 5: slotSetUsePatchesOnOff((int)static_QUType_int.get(_o+1)); break;
    case 6: loadImage(); break;
    case 7: loadImage((QString)static_QUType_QString.get(_o+1)); break;
    case 8: loadImageBBox((QString)static_QUType_QString.get(_o+1),(const Rect&)*((const Rect*)static_QUType_ptr.get(_o+2))); break;
    case 9: loadImageSet(); break;
    case 10: loadIDLImageSet(); break;
    case 11: loadIDLImageSet((const QString&)static_QUType_QString.get(_o+1)); break;
    case 12: processImage((QString)static_QUType_QString.get(_o+1)); break;
    case 13: processImageBBox((QString)static_QUType_QString.get(_o+1),(const Rect&)*((const Rect*)static_QUType_ptr.get(_o+2))); break;
    case 14: collectPatches(); break;
    case 15: collectPatches((bool)static_QUType_bool.get(_o+1)); break;
    case 16: drawInterestPoints(); break;
    case 17: drawInterestPointsEllipse(); break;
    case 18: displayPatchesForBrowsing((vector<OpGrayImage>&)*((vector<OpGrayImage>*)static_QUType_ptr.get(_o+1))); break;
    case 19: clusterPatches(); break;
    case 20: remove1PatchClusters(); break;
    case 21: removeClutteredPatches(); break;
    case 22: recreateCodebookFromTrace(); break;
    case 23: loadCodebook(); break;
    case 24: loadCodebook((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 25: appendCodebook(); break;
    case 26: appendCodebook((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 27: saveCodebook(); break;
    case 28: saveCodebook((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 29: computeFVEigenspace(); break;
    case 30: loadESMatlab(); break;
    case 31: displayEigenspace((const vector<FeatureVector>&)*((const vector<FeatureVector>*)static_QUType_ptr.get(_o+1)),(const FeatureVector&)*((const FeatureVector*)static_QUType_ptr.get(_o+2)),(const FeatureVector&)*((const FeatureVector*)static_QUType_ptr.get(_o+3))); break;
    case 32: savePatchesMatlab(); break;
    case 33: saveFeaturesMatlab(); break;
    case 34: saveClustersMatlab(); break;
    case 35: saveFeatureLocations(); break;
    case 36: saveFeatureLocations((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 37: saveFeatureClasses(); break;
    case 38: saveFeatureClasses((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 39: loadFeatureClasses((string)(*((string*)static_QUType_ptr.get(_o+1))),(vector<int>&)*((vector<int>*)static_QUType_ptr.get(_o+2))); break;
    case 40: saveClusterTrace(); break;
    case 41: saveClusteredImages(); break;
    case 42: static_QUType_bool.set(_o,readVector((string)(*((string*)static_QUType_ptr.get(_o+1))),(FeatureVector&)*((FeatureVector*)static_QUType_ptr.get(_o+2)))); break;
    case 43: static_QUType_bool.set(_o,readMatrix((string)(*((string*)static_QUType_ptr.get(_o+1))),(vector<FeatureVector>&)*((vector<FeatureVector>*)static_QUType_ptr.get(_o+2)))); break;
    case 44: loadParams(); break;
    case 45: loadParams((bool)static_QUType_bool.get(_o+1)); break;
    case 46: loadParams((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 47: loadParams((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    case 48: saveParams(); break;
    case 49: saveParams((bool)static_QUType_bool.get(_o+1)); break;
    case 50: saveParams((string)(*((string*)static_QUType_ptr.get(_o+1)))); break;
    case 51: saveParams((string)(*((string*)static_QUType_ptr.get(_o+1))),(bool)static_QUType_bool.get(_o+2)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Clusterer::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigObjWidthChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: sigObjHeightChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool Clusterer::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool Clusterer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
