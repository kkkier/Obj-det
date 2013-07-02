/****************************************************************************
** Meta object code from reading C++ file 'featExtract.hh'
**
** Created: Sun Jun 30 21:26:12 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "featExtract.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'featExtract.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FeatExtract[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      28,   12,   12,   12, 0x0a,
      45,   12,   12,   12, 0x0a,
      55,   12,   12,   12, 0x0a,
      69,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FeatExtract[] = {
    "FeatExtract\0\0loadImageSet()\0"
    "loadFeatureSet()\0cluster()\0loadCenters()\0"
    "getAssignmentandSave()\0"
};

void FeatExtract::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FeatExtract *_t = static_cast<FeatExtract *>(_o);
        switch (_id) {
        case 0: _t->loadImageSet(); break;
        case 1: _t->loadFeatureSet(); break;
        case 2: _t->cluster(); break;
        case 3: _t->loadCenters(); break;
        case 4: _t->getAssignmentandSave(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData FeatExtract::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FeatExtract::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FeatExtract,
      qt_meta_data_FeatExtract, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FeatExtract::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FeatExtract::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FeatExtract::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FeatExtract))
        return static_cast<void*>(const_cast< FeatExtract*>(this));
    return QWidget::qt_metacast(_clname);
}

int FeatExtract::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
