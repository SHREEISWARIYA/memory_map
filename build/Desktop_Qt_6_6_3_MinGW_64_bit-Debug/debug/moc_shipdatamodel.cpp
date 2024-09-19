/****************************************************************************
** Meta object code from reading C++ file 'shipdatamodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../shipdatamodel.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'shipdatamodel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSShipDataModelENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSShipDataModelENDCLASS = QtMocHelpers::stringData(
    "ShipDataModel",
    "isLoadingChanged",
    "",
    "totalShipsChanged",
    "currentPageChanged",
    "itemsPerPageChanged",
    "fetchShipData",
    "updatePage",
    "isLoading",
    "totalShips",
    "currentPage",
    "itemsPerPage",
    "tableModel",
    "ShipTableModel*"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSShipDataModelENDCLASS_t {
    uint offsetsAndSizes[28];
    char stringdata0[14];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[19];
    char stringdata5[20];
    char stringdata6[14];
    char stringdata7[11];
    char stringdata8[10];
    char stringdata9[11];
    char stringdata10[12];
    char stringdata11[13];
    char stringdata12[11];
    char stringdata13[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSShipDataModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSShipDataModelENDCLASS_t qt_meta_stringdata_CLASSShipDataModelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "ShipDataModel"
        QT_MOC_LITERAL(14, 16),  // "isLoadingChanged"
        QT_MOC_LITERAL(31, 0),  // ""
        QT_MOC_LITERAL(32, 17),  // "totalShipsChanged"
        QT_MOC_LITERAL(50, 18),  // "currentPageChanged"
        QT_MOC_LITERAL(69, 19),  // "itemsPerPageChanged"
        QT_MOC_LITERAL(89, 13),  // "fetchShipData"
        QT_MOC_LITERAL(103, 10),  // "updatePage"
        QT_MOC_LITERAL(114, 9),  // "isLoading"
        QT_MOC_LITERAL(124, 10),  // "totalShips"
        QT_MOC_LITERAL(135, 11),  // "currentPage"
        QT_MOC_LITERAL(147, 12),  // "itemsPerPage"
        QT_MOC_LITERAL(160, 10),  // "tableModel"
        QT_MOC_LITERAL(171, 15)   // "ShipTableModel*"
    },
    "ShipDataModel",
    "isLoadingChanged",
    "",
    "totalShipsChanged",
    "currentPageChanged",
    "itemsPerPageChanged",
    "fetchShipData",
    "updatePage",
    "isLoading",
    "totalShips",
    "currentPage",
    "itemsPerPage",
    "tableModel",
    "ShipTableModel*"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSShipDataModelENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       5,   56, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x06,    6 /* Public */,
       3,    0,   51,    2, 0x06,    7 /* Public */,
       4,    0,   52,    2, 0x06,    8 /* Public */,
       5,    0,   53,    2, 0x06,    9 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   54,    2, 0x02,   10 /* Public */,
       7,    0,   55,    2, 0x02,   11 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       8, QMetaType::Bool, 0x00015001, uint(0), 0,
       9, QMetaType::Int, 0x00015001, uint(1), 0,
      10, QMetaType::Int, 0x00015103, uint(2), 0,
      11, QMetaType::Int, 0x00015103, uint(3), 0,
      12, 0x80000000 | 13, 0x00015409, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject ShipDataModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSShipDataModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSShipDataModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSShipDataModelENDCLASS_t,
        // property 'isLoading'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'totalShips'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'currentPage'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'itemsPerPage'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'tableModel'
        QtPrivate::TypeAndForceComplete<ShipTableModel*, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ShipDataModel, std::true_type>,
        // method 'isLoadingChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'totalShipsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'currentPageChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'itemsPerPageChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fetchShipData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updatePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ShipDataModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ShipDataModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->isLoadingChanged(); break;
        case 1: _t->totalShipsChanged(); break;
        case 2: _t->currentPageChanged(); break;
        case 3: _t->itemsPerPageChanged(); break;
        case 4: _t->fetchShipData(); break;
        case 5: _t->updatePage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ShipDataModel::*)();
            if (_t _q_method = &ShipDataModel::isLoadingChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ShipDataModel::*)();
            if (_t _q_method = &ShipDataModel::totalShipsChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ShipDataModel::*)();
            if (_t _q_method = &ShipDataModel::currentPageChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ShipDataModel::*)();
            if (_t _q_method = &ShipDataModel::itemsPerPageChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ShipTableModel* >(); break;
        }
    }  else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ShipDataModel *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isLoading(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->totalShips(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->currentPage(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->itemsPerPage(); break;
        case 4: *reinterpret_cast< ShipTableModel**>(_v) = _t->tableModel(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ShipDataModel *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setCurrentPage(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setItemsPerPage(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *ShipDataModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShipDataModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSShipDataModelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ShipDataModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ShipDataModel::isLoadingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ShipDataModel::totalShipsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ShipDataModel::currentPageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ShipDataModel::itemsPerPageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
