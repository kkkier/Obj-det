/****************************************************************************
** Meta object code from reading C++ file 'qtcoordlabel.hh'
**
** Created: Tue Feb 5 11:28:03 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qtcoordlabel.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtcoordlabel.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtCoordLabel[] = {

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
      19,   14,   13,   13, 0x0a,
      39,   13,   13,   13, 0x0a,
      60,   58,   13,   13, 0x0a,
      86,   83,   13,   13, 0x0a,
     118,  114,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtCoordLabel[] = {
    "QtCoordLabel\0\0text\0setCaption(QString)\0"
    "setCoordValue(int)\0,\0setCoordValue(int,int)\0"
    ",,\0setCoordValue(int,int,QRgb)\0,,,\0"
    "setCoordValue(int,int,QRgb,float)\0"
};

void QtCoordLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtCoordLabel *_t = static_cast<QtCoordLabel *>(_o);
        switch (_id) {
        case 0: _t->setCaption((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setCoordValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setCoordValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->setCoordValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QRgb(*)>(_a[3]))); break;
        case 4: _t->setCoordValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QRgb(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtCoordLabel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtCoordLabel::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_QtCoordLabel,
      qt_meta_data_QtCoordLabel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtCoordLabel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtCoordLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtCoordLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtCoordLabel))
        return static_cast<void*>(const_cast< QtCoordLabel*>(this));
    return QLabel::qt_metacast(_clname);
}

int QtCoordLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
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
