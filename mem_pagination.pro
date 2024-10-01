QT += quick  concurrent widgets location positioning
QTPLUGIN += qml_QtGraphicalEffects

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        BackgroundProcessor.cpp \
        UserSettings.cpp \
        cursorcontroller.cpp \
        flagimg.cpp \
        main.cpp \
        messagetype.cpp \
        pasttrail.cpp \
        shipdata.cpp \
        shipdatamodel.cpp \
        shiptablemodel.cpp \
        EncConvertor.cpp \
        ShapefileManager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =


INCLUDEPATH += C:/OSGeo4W/include

LIBS += -L"C:/OSGeo4W/lib" -lgdal

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += C:/OSGeo4W/include
LIBS += -LC:\OSGeo4W\lib -lgdal_i

HEADERS += \
    BackgroundProcessor.h \
    UserSettings.h \
    cursorcontroller.h \
    flagimg.h \
    messagetype.h \
    pasttrail.h \
    shipdata.h \
    shipdatamodel.h \
    shiptablemodel.h \
    EncConvertor.h \
    ShapefileManager.h

DISTFILES += \
    CountryName_MMSI_CountryCode.csv \
    countryFlag_svg/LICENSE.txt \
    countryFlag_svg/ObtainedFrom.txt \
    countryFlag_svg/ab.svg \
    countryFlag_svg/ad.svg \
    countryFlag_svg/ae.svg \
    countryFlag_svg/af.svg \
    countryFlag_svg/ag.svg \
    countryFlag_svg/ai.svg \
    countryFlag_svg/al.svg \
    countryFlag_svg/am.svg \
    countryFlag_svg/ao.svg \
    countryFlag_svg/aq.svg \
    countryFlag_svg/ar.svg \
    countryFlag_svg/as.svg \
    countryFlag_svg/at.svg \
    countryFlag_svg/au.svg \
    countryFlag_svg/aw.svg \
    countryFlag_svg/ax.svg \
    countryFlag_svg/az.svg \
    countryFlag_svg/ba.svg \
    countryFlag_svg/bb.svg \
    countryFlag_svg/bd.svg \
    countryFlag_svg/be.svg \
    countryFlag_svg/bf.svg \
    countryFlag_svg/bg.svg \
    countryFlag_svg/bh.svg \
    countryFlag_svg/bi.svg \
    countryFlag_svg/bj.svg \
    countryFlag_svg/bl.svg \
    countryFlag_svg/bm.svg \
    countryFlag_svg/bn.svg \
    countryFlag_svg/bo.svg \
    countryFlag_svg/bq.svg \
    countryFlag_svg/br.svg \
    countryFlag_svg/bs.svg \
    countryFlag_svg/bt.svg \
    countryFlag_svg/bv.svg \
    countryFlag_svg/bw.svg \
    countryFlag_svg/by.svg \
    countryFlag_svg/bz.svg \
    countryFlag_svg/ca.svg \
    countryFlag_svg/cc.svg \
    countryFlag_svg/cd.svg \
    countryFlag_svg/cf.svg \
    countryFlag_svg/cg.svg \
    countryFlag_svg/ch.svg \
    countryFlag_svg/ci.svg \
    countryFlag_svg/ck.svg \
    countryFlag_svg/cl.svg \
    countryFlag_svg/cm.svg \
    countryFlag_svg/cn.svg \
    countryFlag_svg/co.svg \
    countryFlag_svg/cr.svg \
    countryFlag_svg/cu.svg \
    countryFlag_svg/cv.svg \
    countryFlag_svg/cw.svg \
    countryFlag_svg/cx.svg \
    countryFlag_svg/cy.svg \
    countryFlag_svg/cz.svg \
    countryFlag_svg/de.svg \
    countryFlag_svg/dj.svg \
    countryFlag_svg/dk.svg \
    countryFlag_svg/dm.svg \
    countryFlag_svg/do.svg \
    countryFlag_svg/dz.svg \
    countryFlag_svg/ec.svg \
    countryFlag_svg/ee.svg \
    countryFlag_svg/eg.svg \
    countryFlag_svg/eh.svg \
    countryFlag_svg/england.svg \
    countryFlag_svg/er.svg \
    countryFlag_svg/es.svg \
    countryFlag_svg/et.svg \
    countryFlag_svg/europeanunion.svg \
    countryFlag_svg/fi.svg \
    countryFlag_svg/filelist.txt \
    countryFlag_svg/fj.svg \
    countryFlag_svg/fk.svg \
    countryFlag_svg/fm.svg \
    countryFlag_svg/fo.svg \
    countryFlag_svg/fr.svg \
    countryFlag_svg/ga.svg \
    countryFlag_svg/gb.svg \
    countryFlag_svg/gd.svg \
    countryFlag_svg/ge.svg \
    countryFlag_svg/gf.svg \
    countryFlag_svg/gg.svg \
    countryFlag_svg/gh.svg \
    countryFlag_svg/gi.svg \
    countryFlag_svg/gl.svg \
    countryFlag_svg/gm.svg \
    countryFlag_svg/gn.svg \
    countryFlag_svg/gp.svg \
    countryFlag_svg/gq.svg \
    countryFlag_svg/gr.svg \
    countryFlag_svg/gs.svg \
    countryFlag_svg/gt.svg \
    countryFlag_svg/gu.svg \
    countryFlag_svg/gw.svg \
    countryFlag_svg/gy.svg \
    countryFlag_svg/hk.svg \
    countryFlag_svg/hm.svg \
    countryFlag_svg/hn.svg \
    countryFlag_svg/hr.svg \
    countryFlag_svg/ht.svg \
    countryFlag_svg/hu.svg \
    countryFlag_svg/id.svg \
    countryFlag_svg/ie.svg \
    countryFlag_svg/il.svg \
    countryFlag_svg/im.svg \
    countryFlag_svg/in.svg \
    countryFlag_svg/io.svg \
    countryFlag_svg/iq.svg \
    countryFlag_svg/ir.svg \
    countryFlag_svg/is.svg \
    countryFlag_svg/it.svg \
    countryFlag_svg/je.svg \
    countryFlag_svg/jm.svg \
    countryFlag_svg/jo.svg \
    countryFlag_svg/jp.svg \
    countryFlag_svg/ke.svg \
    countryFlag_svg/kg.svg \
    countryFlag_svg/kh.svg \
    countryFlag_svg/ki.svg \
    countryFlag_svg/km.svg \
    countryFlag_svg/kn.svg \
    countryFlag_svg/kp.svg \
    countryFlag_svg/kr.svg \
    countryFlag_svg/kw.svg \
    countryFlag_svg/ky.svg \
    countryFlag_svg/kz.svg \
    countryFlag_svg/la.svg \
    countryFlag_svg/lb.svg \
    countryFlag_svg/lc.svg \
    countryFlag_svg/li.svg \
    countryFlag_svg/lk.svg \
    countryFlag_svg/lr.svg \
    countryFlag_svg/ls.svg \
    countryFlag_svg/lt.svg \
    countryFlag_svg/lu.svg \
    countryFlag_svg/lv.svg \
    countryFlag_svg/ly.svg \
    countryFlag_svg/ma.svg \
    countryFlag_svg/mc.svg \
    countryFlag_svg/md.svg \
    countryFlag_svg/me.svg \
    countryFlag_svg/mf.svg \
    countryFlag_svg/mg.svg \
    countryFlag_svg/mh.svg \
    countryFlag_svg/mk.svg \
    countryFlag_svg/ml.svg \
    countryFlag_svg/mm.svg \
    countryFlag_svg/mn.svg \
    countryFlag_svg/mo.svg \
    countryFlag_svg/mp.svg \
    countryFlag_svg/mq.svg \
    countryFlag_svg/mr.svg \
    countryFlag_svg/ms.svg \
    countryFlag_svg/mt.svg \
    countryFlag_svg/mu.svg \
    countryFlag_svg/mv.svg \
    countryFlag_svg/mw.svg \
    countryFlag_svg/mx.svg \
    countryFlag_svg/my.svg \
    countryFlag_svg/mz.svg \
    countryFlag_svg/na.svg \
    countryFlag_svg/nc.svg \
    countryFlag_svg/ne.svg \
    countryFlag_svg/nf.svg \
    countryFlag_svg/ng.svg \
    countryFlag_svg/ni.svg \
    countryFlag_svg/nl.svg \
    countryFlag_svg/no.svg \
    countryFlag_svg/northerncyprus.svg \
    countryFlag_svg/np.svg \
    countryFlag_svg/nr.svg \
    countryFlag_svg/nu.svg \
    countryFlag_svg/nz.svg \
    countryFlag_svg/om.svg \
    countryFlag_svg/pa.svg \
    countryFlag_svg/pe.svg \
    countryFlag_svg/pf.svg \
    countryFlag_svg/pg.svg \
    countryFlag_svg/ph.svg \
    countryFlag_svg/pk.svg \
    countryFlag_svg/pl.svg \
    countryFlag_svg/pm.svg \
    countryFlag_svg/pn.svg \
    countryFlag_svg/pr.svg \
    countryFlag_svg/ps.svg \
    countryFlag_svg/pt.svg \
    countryFlag_svg/pw.svg \
    countryFlag_svg/py.svg \
    countryFlag_svg/qa.svg \
    countryFlag_svg/re.svg \
    countryFlag_svg/ro.svg \
    countryFlag_svg/rs.svg \
    countryFlag_svg/ru.svg \
    countryFlag_svg/rw.svg \
    countryFlag_svg/sa.svg \
    countryFlag_svg/sb.svg \
    countryFlag_svg/sc.svg \
    countryFlag_svg/scotland.svg \
    countryFlag_svg/sd.svg \
    countryFlag_svg/se.svg \
    countryFlag_svg/sg.svg \
    countryFlag_svg/sh.svg \
    countryFlag_svg/si.svg \
    countryFlag_svg/sj.svg \
    countryFlag_svg/sk.svg \
    countryFlag_svg/sl.svg \
    countryFlag_svg/sm.svg \
    countryFlag_svg/sn.svg \
    countryFlag_svg/so.svg \
    countryFlag_svg/southossetia.svg \
    countryFlag_svg/sr.svg \
    countryFlag_svg/ss.svg \
    countryFlag_svg/st.svg \
    countryFlag_svg/sv.svg \
    countryFlag_svg/sx.svg \
    countryFlag_svg/sy.svg \
    countryFlag_svg/sz.svg \
    countryFlag_svg/tc.svg \
    countryFlag_svg/td.svg \
    countryFlag_svg/tf.svg \
    countryFlag_svg/tg.svg \
    countryFlag_svg/th.svg \
    countryFlag_svg/tj.svg \
    countryFlag_svg/tk.svg \
    countryFlag_svg/tl.svg \
    countryFlag_svg/tm.svg \
    countryFlag_svg/tn.svg \
    countryFlag_svg/to.svg \
    countryFlag_svg/tr.svg \
    countryFlag_svg/tt.svg \
    countryFlag_svg/tv.svg \
    countryFlag_svg/tw.svg \
    countryFlag_svg/tz.svg \
    countryFlag_svg/ua.svg \
    countryFlag_svg/ug.svg \
    countryFlag_svg/um.svg \
    countryFlag_svg/unitednations.svg \
    countryFlag_svg/us.svg \
    countryFlag_svg/uy.svg \
    countryFlag_svg/uz.svg \
    countryFlag_svg/va.svg \
    countryFlag_svg/vc.svg \
    countryFlag_svg/ve.svg \
    countryFlag_svg/vg.svg \
    countryFlag_svg/vi.svg \
    countryFlag_svg/vn.svg \
    countryFlag_svg/vu.svg \
    countryFlag_svg/wales.svg \
    countryFlag_svg/wf.svg \
    countryFlag_svg/ws.svg \
    countryFlag_svg/xk.svg \
    countryFlag_svg/ye.svg \
    countryFlag_svg/yt.svg \
    countryFlag_svg/za.svg \
    countryFlag_svg/zm.svg \
    countryFlag_svg/zw.svg
