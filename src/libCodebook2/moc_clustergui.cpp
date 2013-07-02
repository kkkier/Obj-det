/****************************************************************************
** ClusterGUI meta object code from reading C++ file 'clustergui.hh'
**
** Created: Wed Jan 30 12:05:32 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "clustergui.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ClusterGUI::className() const
{
    return "ClusterGUI";
}

QMetaObject *ClusterGUI::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ClusterGUI( "ClusterGUI", &ClusterGUI::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ClusterGUI::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClusterGUI", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ClusterGUI::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClusterGUI", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ClusterGUI::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotSetEnergy", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"slotSetNumClusters", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"slotSetEps", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"slotSetMaxIter", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotSetSimilarity", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"slotSetFeatureSimFact", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"slotSetMaxNodeSize", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"slotSetCompressionRatio", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"slotSetSimLevel", 1, param_slot_8 };
    static const QUMethod slot_9 = {"slotUpdateEnergy", 0, 0 };
    static const QUMethod slot_10 = {"slotUpdateNumClusters", 0, 0 };
    static const QUMethod slot_11 = {"slotUpdateEps", 0, 0 };
    static const QUMethod slot_12 = {"slotUpdateMaxIter", 0, 0 };
    static const QUMethod slot_13 = {"slotUpdateSimilarity", 0, 0 };
    static const QUMethod slot_14 = {"slotUpdateFeatureSimFact", 0, 0 };
    static const QUMethod slot_15 = {"slotUpdateMaxNodeSize", 0, 0 };
    static const QUMethod slot_16 = {"slotUpdateCompressionRatio", 0, 0 };
    static const QUMethod slot_17 = {"slotUpdateSimLevel", 0, 0 };
    static const QUParameter param_slot_18[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"slotSelectClusterMethod", 1, param_slot_18 };
    static const QUParameter param_slot_19[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_19 = {"slotSelectReconstMethod", 1, param_slot_19 };
    static const QUParameter param_slot_20[] = {
	{ "state", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"slotSetPCAOnOff", 1, param_slot_20 };
    static const QUParameter param_slot_21[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_21 = {"slotSetTreeNum", 1, param_slot_21 };
    static const QUParameter param_slot_22[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_22 = {"slotSetTreeDepth", 1, param_slot_22 };
    static const QUParameter param_slot_23[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_23 = {"slotSetWeekClassifier", 1, param_slot_23 };
    static const QUParameter param_slot_24[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_24 = {"slotSetMinSample", 1, param_slot_24 };
    static const QUMethod slot_25 = {"slotUpdateTreeNum", 0, 0 };
    static const QUMethod slot_26 = {"slotUpdateTreeDepth", 0, 0 };
    static const QUMethod slot_27 = {"slotUpdateWeekClassifier", 0, 0 };
    static const QUMethod slot_28 = {"slotUpdateMinSample", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "slotSetEnergy(const QString&)", &slot_0, QMetaData::Public },
	{ "slotSetNumClusters(const QString&)", &slot_1, QMetaData::Public },
	{ "slotSetEps(const QString&)", &slot_2, QMetaData::Public },
	{ "slotSetMaxIter(const QString&)", &slot_3, QMetaData::Public },
	{ "slotSetSimilarity(const QString&)", &slot_4, QMetaData::Public },
	{ "slotSetFeatureSimFact(const QString&)", &slot_5, QMetaData::Public },
	{ "slotSetMaxNodeSize(const QString&)", &slot_6, QMetaData::Public },
	{ "slotSetCompressionRatio(const QString&)", &slot_7, QMetaData::Public },
	{ "slotSetSimLevel(const QString&)", &slot_8, QMetaData::Public },
	{ "slotUpdateEnergy()", &slot_9, QMetaData::Public },
	{ "slotUpdateNumClusters()", &slot_10, QMetaData::Public },
	{ "slotUpdateEps()", &slot_11, QMetaData::Public },
	{ "slotUpdateMaxIter()", &slot_12, QMetaData::Public },
	{ "slotUpdateSimilarity()", &slot_13, QMetaData::Public },
	{ "slotUpdateFeatureSimFact()", &slot_14, QMetaData::Public },
	{ "slotUpdateMaxNodeSize()", &slot_15, QMetaData::Public },
	{ "slotUpdateCompressionRatio()", &slot_16, QMetaData::Public },
	{ "slotUpdateSimLevel()", &slot_17, QMetaData::Public },
	{ "slotSelectClusterMethod(int)", &slot_18, QMetaData::Public },
	{ "slotSelectReconstMethod(int)", &slot_19, QMetaData::Public },
	{ "slotSetPCAOnOff(int)", &slot_20, QMetaData::Public },
	{ "slotSetTreeNum(const QString&)", &slot_21, QMetaData::Public },
	{ "slotSetTreeDepth(const QString&)", &slot_22, QMetaData::Public },
	{ "slotSetWeekClassifier(const QString&)", &slot_23, QMetaData::Public },
	{ "slotSetMinSample(const QString&)", &slot_24, QMetaData::Public },
	{ "slotUpdateTreeNum()", &slot_25, QMetaData::Public },
	{ "slotUpdateTreeDepth()", &slot_26, QMetaData::Public },
	{ "slotUpdateWeekClassifier()", &slot_27, QMetaData::Public },
	{ "slotUpdateMinSample()", &slot_28, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigNumClustersChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"sigEpsChanged", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"sigMaxIterChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"sigVarianceChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"sigSimilarityChanged", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"sigFeatureSimFactChanged", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_6 = {"sigMaxNodeSizeChanged", 1, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"sigCompressionRatioChanged", 1, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_8 = {"sigSimLevelChanged", 1, param_signal_8 };
    static const QUParameter param_signal_9[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_9 = {"sigFileNameChanged", 1, param_signal_9 };
    static const QUParameter param_signal_10[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_10 = {"sigEnergyChanged", 1, param_signal_10 };
    static const QUParameter param_signal_11[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_11 = {"sigTreeNumChanged", 1, param_signal_11 };
    static const QUParameter param_signal_12[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_12 = {"sigTreeDepthChanged", 1, param_signal_12 };
    static const QUParameter param_signal_13[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_13 = {"sigWeekClassifierChanged", 1, param_signal_13 };
    static const QUParameter param_signal_14[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_14 = {"sigMinSampleChanged", 1, param_signal_14 };
    static const QMetaData signal_tbl[] = {
	{ "sigNumClustersChanged(const QString&)", &signal_0, QMetaData::Public },
	{ "sigEpsChanged(const QString&)", &signal_1, QMetaData::Public },
	{ "sigMaxIterChanged(const QString&)", &signal_2, QMetaData::Public },
	{ "sigVarianceChanged(const QString&)", &signal_3, QMetaData::Public },
	{ "sigSimilarityChanged(const QString&)", &signal_4, QMetaData::Public },
	{ "sigFeatureSimFactChanged(const QString&)", &signal_5, QMetaData::Public },
	{ "sigMaxNodeSizeChanged(const QString&)", &signal_6, QMetaData::Public },
	{ "sigCompressionRatioChanged(const QString&)", &signal_7, QMetaData::Public },
	{ "sigSimLevelChanged(const QString&)", &signal_8, QMetaData::Public },
	{ "sigFileNameChanged(const QString&)", &signal_9, QMetaData::Public },
	{ "sigEnergyChanged(const QString&)", &signal_10, QMetaData::Public },
	{ "sigTreeNumChanged(const QString&)", &signal_11, QMetaData::Public },
	{ "sigTreeDepthChanged(const QString&)", &signal_12, QMetaData::Public },
	{ "sigWeekClassifierChanged(const QString&)", &signal_13, QMetaData::Public },
	{ "sigMinSampleChanged(const QString&)", &signal_14, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ClusterGUI", parentObject,
	slot_tbl, 29,
	signal_tbl, 15,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ClusterGUI.setMetaObject( metaObj );
    return metaObj;
}

void* ClusterGUI::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ClusterGUI" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sigNumClustersChanged
void ClusterGUI::sigNumClustersChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL sigEpsChanged
void ClusterGUI::sigEpsChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL sigMaxIterChanged
void ClusterGUI::sigMaxIterChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL sigVarianceChanged
void ClusterGUI::sigVarianceChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL sigSimilarityChanged
void ClusterGUI::sigSimilarityChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL sigFeatureSimFactChanged
void ClusterGUI::sigFeatureSimFactChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL sigMaxNodeSizeChanged
void ClusterGUI::sigMaxNodeSizeChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 6, t0 );
}

// SIGNAL sigCompressionRatioChanged
void ClusterGUI::sigCompressionRatioChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 7, t0 );
}

// SIGNAL sigSimLevelChanged
void ClusterGUI::sigSimLevelChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 8, t0 );
}

// SIGNAL sigFileNameChanged
void ClusterGUI::sigFileNameChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 9, t0 );
}

// SIGNAL sigEnergyChanged
void ClusterGUI::sigEnergyChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 10, t0 );
}

// SIGNAL sigTreeNumChanged
void ClusterGUI::sigTreeNumChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 11, t0 );
}

// SIGNAL sigTreeDepthChanged
void ClusterGUI::sigTreeDepthChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 12, t0 );
}

// SIGNAL sigWeekClassifierChanged
void ClusterGUI::sigWeekClassifierChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 13, t0 );
}

// SIGNAL sigMinSampleChanged
void ClusterGUI::sigMinSampleChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 14, t0 );
}

bool ClusterGUI::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotSetEnergy((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: slotSetNumClusters((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: slotSetEps((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: slotSetMaxIter((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: slotSetSimilarity((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: slotSetFeatureSimFact((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: slotSetMaxNodeSize((const QString&)static_QUType_QString.get(_o+1)); break;
    case 7: slotSetCompressionRatio((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: slotSetSimLevel((const QString&)static_QUType_QString.get(_o+1)); break;
    case 9: slotUpdateEnergy(); break;
    case 10: slotUpdateNumClusters(); break;
    case 11: slotUpdateEps(); break;
    case 12: slotUpdateMaxIter(); break;
    case 13: slotUpdateSimilarity(); break;
    case 14: slotUpdateFeatureSimFact(); break;
    case 15: slotUpdateMaxNodeSize(); break;
    case 16: slotUpdateCompressionRatio(); break;
    case 17: slotUpdateSimLevel(); break;
    case 18: slotSelectClusterMethod((int)static_QUType_int.get(_o+1)); break;
    case 19: slotSelectReconstMethod((int)static_QUType_int.get(_o+1)); break;
    case 20: slotSetPCAOnOff((int)static_QUType_int.get(_o+1)); break;
    case 21: slotSetTreeNum((const QString&)static_QUType_QString.get(_o+1)); break;
    case 22: slotSetTreeDepth((const QString&)static_QUType_QString.get(_o+1)); break;
    case 23: slotSetWeekClassifier((const QString&)static_QUType_QString.get(_o+1)); break;
    case 24: slotSetMinSample((const QString&)static_QUType_QString.get(_o+1)); break;
    case 25: slotUpdateTreeNum(); break;
    case 26: slotUpdateTreeDepth(); break;
    case 27: slotUpdateWeekClassifier(); break;
    case 28: slotUpdateMinSample(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ClusterGUI::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigNumClustersChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: sigEpsChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: sigMaxIterChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: sigVarianceChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: sigSimilarityChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: sigFeatureSimFactChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: sigMaxNodeSizeChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 7: sigCompressionRatioChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: sigSimLevelChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 9: sigFileNameChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 10: sigEnergyChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 11: sigTreeNumChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 12: sigTreeDepthChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 13: sigWeekClassifierChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 14: sigMinSampleChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ClusterGUI::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool ClusterGUI::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
