/****************************************************************************
** Meta object code from reading C++ file 'rf_ismreco.hh'
**
** Created: Mon Jun 17 22:56:30 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "rf_ismreco.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rf_ismreco.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RFISMReco[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      35,   10,   10,   10, 0x0a,
      55,   10,   10,   10, 0x0a,
      78,   10,   10,   10, 0x0a,
      86,   10,   10,   10, 0x0a,
     102,   10,   10,   10, 0x0a,
     118,   10,   10,   10, 0x0a,
     153,   10,   10,   10, 0x0a,
     172,   10,   10,   10, 0x0a,
     191,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RFISMReco[] = {
    "RFISMReco\0\0loadImageSet4Training()\0"
    "loadImage4Testing()\0loadImageSet4Testing()\0"
    "train()\0trainwithpuri()\0rankLeafNodes()\0"
    "showDiscriminateLeafNodeCoverage()\0"
    "saveRandomForest()\0loadRandomForest()\0"
    "test()\0"
};

void RFISMReco::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RFISMReco *_t = static_cast<RFISMReco *>(_o);
        switch (_id) {
        case 0: _t->loadImageSet4Training(); break;
        case 1: _t->loadImage4Testing(); break;
        case 2: _t->loadImageSet4Testing(); break;
        case 3: _t->train(); break;
        case 4: _t->trainwithpuri(); break;
        case 5: _t->rankLeafNodes(); break;
        case 6: _t->showDiscriminateLeafNodeCoverage(); break;
        case 7: _t->saveRandomForest(); break;
        case 8: _t->loadRandomForest(); break;
        case 9: _t->test(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData RFISMReco::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RFISMReco::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RFISMReco,
      qt_meta_data_RFISMReco, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RFISMReco::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RFISMReco::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RFISMReco::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RFISMReco))
        return static_cast<void*>(const_cast< RFISMReco*>(this));
    return QWidget::qt_metacast(_clname);
}

int RFISMReco::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
