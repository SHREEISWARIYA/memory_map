/****************************************************************************
** Meta object code from reading C++ file 'shipdata.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../shipdata.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'shipdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSShipDataENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSShipDataENDCLASS = QtMocHelpers::stringData(
    "ShipData",
    "totalShipsChanged",
    "",
    "shipsChanged",
    "mmsiUuidListChanged",
    "onNetworkReply",
    "QNetworkReply*",
    "reply",
    "fetchShips",
    "isValidShip",
    "uuid",
    "getUuidFromMmsi",
    "mmsi",
    "getShipDetails",
    "printShipDetails",
    "totalShips",
    "shipList",
    "mmsiUuidList"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSShipDataENDCLASS_t {
    uint offsetsAndSizes[36];
    char stringdata0[9];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[20];
    char stringdata5[15];
    char stringdata6[15];
    char stringdata7[6];
    char stringdata8[11];
    char stringdata9[12];
    char stringdata10[5];
    char stringdata11[16];
    char stringdata12[5];
    char stringdata13[15];
    char stringdata14[17];
    char stringdata15[11];
    char stringdata16[9];
    char stringdata17[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSShipDataENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSShipDataENDCLASS_t qt_meta_stringdata_CLASSShipDataENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "ShipData"
        QT_MOC_LITERAL(9, 17),  // "totalShipsChanged"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 12),  // "shipsChanged"
        QT_MOC_LITERAL(41, 19),  // "mmsiUuidListChanged"
        QT_MOC_LITERAL(61, 14),  // "onNetworkReply"
        QT_MOC_LITERAL(76, 14),  // "QNetworkReply*"
        QT_MOC_LITERAL(91, 5),  // "reply"
        QT_MOC_LITERAL(97, 10),  // "fetchShips"
        QT_MOC_LITERAL(108, 11),  // "isValidShip"
        QT_MOC_LITERAL(120, 4),  // "uuid"
        QT_MOC_LITERAL(125, 15),  // "getUuidFromMmsi"
        QT_MOC_LITERAL(141, 4),  // "mmsi"
        QT_MOC_LITERAL(146, 14),  // "getShipDetails"
        QT_MOC_LITERAL(161, 16),  // "printShipDetails"
        QT_MOC_LITERAL(178, 10),  // "totalShips"
        QT_MOC_LITERAL(189, 8),  // "shipList"
        QT_MOC_LITERAL(198, 12)   // "mmsiUuidList"
    },
    "ShipData",
    "totalShipsChanged",
    "",
    "shipsChanged",
    "mmsiUuidListChanged",
    "onNetworkReply",
    "QNetworkReply*",
    "reply",
    "fetchShips",
    "isValidShip",
    "uuid",
    "getUuidFromMmsi",
    "mmsi",
    "getShipDetails",
    "printShipDetails",
    "totalShips",
    "shipList",
    "mmsiUuidList"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSShipDataENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       3,   87, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    4 /* Public */,
       3,    0,   69,    2, 0x06,    5 /* Public */,
       4,    0,   70,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   71,    2, 0x08,    7 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   74,    2, 0x02,    9 /* Public */,
       9,    1,   75,    2, 0x102,   10 /* Public | MethodIsConst  */,
      11,    1,   78,    2, 0x102,   12 /* Public | MethodIsConst  */,
      13,    1,   81,    2, 0x102,   14 /* Public | MethodIsConst  */,
      14,    1,   84,    2, 0x102,   16 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,

 // methods: parameters
    QMetaType::Void,
    QMetaType::QVariantMap, QMetaType::QString,   10,
    QMetaType::QString, QMetaType::QString,   12,
    QMetaType::QVariantMap, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,

 // properties: name, type, flags
      15, QMetaType::Int, 0x00015001, uint(0), 0,
      16, QMetaType::QVariantList, 0x00015001, uint(1), 0,
      17, QMetaType::QVariantList, 0x00015001, uint(2), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject ShipData::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSShipDataENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSShipDataENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSShipDataENDCLASS_t,
        // property 'totalShips'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'shipList'
        QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>,
        // property 'mmsiUuidList'
        QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ShipData, std::true_type>,
        // method 'totalShipsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'shipsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'mmsiUuidListChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNetworkReply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'fetchShips'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'isValidShip'
        QtPrivate::TypeAndForceComplete<QVariantMap, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getUuidFromMmsi'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getShipDetails'
        QtPrivate::TypeAndForceComplete<QVariantMap, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'printShipDetails'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void ShipData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ShipData *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->totalShipsChanged(); break;
        case 1: _t->shipsChanged(); break;
        case 2: _t->mmsiUuidListChanged(); break;
        case 3: _t->onNetworkReply((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 4: _t->fetchShips(); break;
        case 5: { QVariantMap _r = _t->isValidShip((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 6: { QString _r = _t->getUuidFromMmsi((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 7: { QVariantMap _r = _t->getShipDetails((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->printShipDetails((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ShipData::*)();
            if (_t _q_method = &ShipData::totalShipsChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ShipData::*)();
            if (_t _q_method = &ShipData::shipsChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ShipData::*)();
            if (_t _q_method = &ShipData::mmsiUuidListChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    } else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ShipData *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->totalShips(); break;
        case 1: *reinterpret_cast< QVariantList*>(_v) = _t->shipList(); break;
        case 2: *reinterpret_cast< QVariantList*>(_v) = _t->mmsiUuidList(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *ShipData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShipData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSShipDataENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ShipData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ShipData::totalShipsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ShipData::shipsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ShipData::mmsiUuidListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
