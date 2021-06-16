#include <qtphotography.h>

N::PictureDepot:: PictureDepot  (QWidget * parent,Plan * p)
                : TreeWidget    (          parent,       p)
                , EachDepot     ( 100000                  )
                , TotalPictures (3000000                  )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PictureDepot::~PictureDepot (void)
{
}

QSize N::PictureDepot::sizeHint(void) const
{
  if (plan->Booleans["Phone"]) {
    return QSize ( 320 , 240 ) ;
  }                            ;
  return   QSize ( 640 , 480 ) ;
}

void N::PictureDepot::Configure (void)
{
  //////////////////////////////////////////////////
  setFocusPolicy          ( Qt::WheelFocus       ) ;
  setDragDropMode         ( DragDrop             ) ;
  setAlternatingRowColors ( true                 ) ;
  setRootIsDecorated      ( false                ) ;
  setSelectionMode        ( SingleSelection      ) ;
  setColumnCount          ( 4                    ) ;
  //////////////////////////////////////////////////
  setWindowTitle          ( tr("Picture depot")  ) ;
  NewTreeWidgetItem       ( head                 ) ;
  head -> setText         ( 0 , tr("Picture"  )  ) ;
  head -> setText         ( 1 , tr("Quantity" )  ) ;
  head -> setText         ( 2 , tr("Thumb"    )  ) ;
  head -> setText         ( 3 , tr("Quantity" )  ) ;
  assignHeaderItems       ( head                 ) ;
  setAlignment            ( 1 , Qt::AlignRight     |
                                Qt::AlignVCenter ) ;
  setAlignment            ( 3 , Qt::AlignRight     |
                                Qt::AlignVCenter ) ;
  plan->setFont           ( this                 ) ;
  //////////////////////////////////////////////////
}

bool N::PictureDepot::FocusIn(void)
{
  AssignAction ( Label   , windowTitle ( ) ) ;
  LinkAction   ( Refresh , startup     ( ) ) ;
  return true                                ;
}

QStringList N::PictureDepot::Unions(QString created)
{
  QStringList T                                                              ;
  QStringList Q                                                              ;
  QString     E                                                              ;
  int         index = created.indexOf(" union=",0      ,Qt::CaseInsensitive) ;
  if (index<0) return T                                                      ;
  int         endix = created.indexOf(")"      ,index+8,Qt::CaseInsensitive) ;
  if (endix<0) return T                                                      ;
  E = created.mid(index+8,endix-index-8)                                     ;
  Q = E.split(',')                                                           ;
  foreach (E,Q)                                                              {
    E  = E.replace("`","")                                                   ;
    T << E                                                                   ;
  }                                                                          ;
  return T                                                                   ;
}

bool N::PictureDepot::startup(void)
{
  clear    (            )                             ;
  EnterSQL ( SC , Depot )                             ;
    QString     Q                                     ;
    QStringList T = SC.sql.tables(SC.DB)              ;
    QStringList P                                     ;
    if (T.contains("picturedepot"))                   {
      Q = QString("show create table picturedepot ;") ;
      if (SC.Fetch(Q))                                {
        QString Z = SC.String(1)                      ;
        P = Unions(Z)                                 ;
      }                                               ;
    }                                                 ;
    if (T.contains("thumbdepot"  ))                   {
      Q = QString("show create table thumbdepot ;")   ;
      if (SC.Fetch(Q))                                {
        QString Z = SC.String(1)                      ;
        T = Unions(Z)                                 ;
      }                                               ;
    }                                                 ;
    if (P.count()==T.count())                         {
      int t = P.count()                               ;
      for (int i=0;i<t;i++)                           {
        int pc = SC.count(P[i])                       ;
        int tc = SC.count(T[i])                       ;
        NewTreeWidgetItem ( IT                      ) ;
        IT -> setText     ( 0 , P[i]                ) ;
        IT -> setText     ( 1 , QString::number(pc) ) ;
        IT -> setText     ( 2 , T[i]                ) ;
        IT -> setText     ( 3 , QString::number(tc) ) ;
        setAlignments     ( IT                      ) ;
        addTopLevelItem   ( IT                      ) ;
      }                                               ;
      Pictures = P                                    ;
      Thumbs   = T                                    ;
    }                                                 ;
    Alert  ( Done       )                             ;
  ErrorSQL ( SC , Depot )                             ;
    Alert  ( Error      )                             ;
  LeaveSQL ( SC , Depot )                             ;
  return true                                         ;
}

bool N::PictureDepot::Menu(QPoint pos)
{
  nScopedMenu ( mm , this )                                                  ;
  QAction * aa                                                               ;
  mm . add          ( 101 , tr("Refresh" ) )                                 ;
  mm . add          ( 102 , tr("Gather"  ) )                                 ;
  mm . add          ( 103 , tr("Optimize") )                                 ;
  mm . setFont      ( plan                 )                                 ;
  if (plan->Booleans["Desktop"])                                             {
    pos = QCursor::pos()                                                     ;
  } else                                                                     {
    pos = mapToGlobal ( pos )                                                ;
  }                                                                          ;
  aa = mm.exec      ( pos                  )                                 ;
  if (!plan->Booleans["Desktop"])                                            {
    allowGesture = true                                                      ;
  }                                                                          ;
  nKickOut          ( IsNull(aa) , true    )                                 ;
  switch (mm[aa])                                                            {
    nFastCast  ( 101 , startup ( ) )                                         ;
    case 102                                                                 :
      ProgressId [ 102 ] = plan->Progress(tr("Gather pictures"),tr("%v/%m")) ;
      start      ( 102 )                                                     ;
    break                                                                    ;
    case 103                                                                 :
      ProgressId [ 103 ] = plan->Progress(tr("Optimize depot" ),tr("%v/%m")) ;
      start      ( 103 )                                                     ;
    break                                                                    ;
  }                                                                          ;
  return true                                                                ;
}

void N::PictureDepot::run(int type,ThreadData * data)
{ Q_UNUSED ( data ) ;
  switch   ( type ) {
    case 102        :
      Gather   ( )  ;
    break           ;
    case 103        :
      Optimize ( )  ;
    break           ;
  }                 ;
}

void N::PictureDepot::Gather(void)
{
  qint64 index                                                    ;
  SqlConnection SC ( Depot )                                      ;
  if (SC.open("PictureDepot","Gather"))                           {
    QString Q                                                     ;
    QString ET                                                    ;
    bool    go                                                    ;
    bool    keep                                                  ;
    qint64  total                                                 ;
    int     target                                                ;
    int     vacancy                                               ;
    int     remnants                                              ;
    ///////////////////////////////////////////////////////////////
    go      = true                                                ;
    total   = Pictures.count()                                    ;
    target  = 0                                                   ;
    vacancy = 0                                                   ;
    index   = 0                                                   ;
    ///////////////////////////////////////////////////////////////
    do                                                            {
      ET       = Pictures [ total-1 ]                             ;
      remnants = SC.count(ET)                                     ;
      keep     = ( remnants > 0 )                                 ;
      if ( target < ( total - 1 ) )                               {
        bool locating = true                                      ;
        do                                                        {
          QString T = Pictures[target]                            ;
          int     t = SC.count(T)                                 ;
          if (t>=EachDepot)                                       {
            target++                                              ;
            if ( target >= ( total - 1) ) locating = false        ;
          } else                                                  {
            vacancy  = EachDepot - t                              ;
            if (vacancy>0) locating = false                       ;
          }                                                       ;
        } while (locating)                                        ;
        if (vacancy<=0) keep = false                              ;
      } else keep = false                                         ;
      if ( keep )                                                 {
        QString MT     = Pictures[target]                         ;
        int     moving = remnants                                 ;
        UUIDs   Uuids                                             ;
        if (moving>vacancy) moving = vacancy                      ;
        plan -> Start    ( ProgressId [ 102 ] , &index , &go    ) ;
        plan -> setRange ( ProgressId [ 102 ] , 0      , moving ) ;
        Uuids  = SC.Uuids(ET,"uuid",SC.sql.Limit(0,moving))       ;
        index  = 0                                                ;
        moving = Uuids.count()                                    ;
        while (go && index<moving)                                {
          QString M                                               ;
          SUID    u = Uuids[index]                                ;
          index++                                                 ;
          Q = QString("insert into %1 (id,uuid,file,ltime) "
                      "select id,uuid,file,ltime from %2 where uuid = %3 ;")
              .arg(MT).arg(ET).arg(u)                             ;
          M = tr("Move %1 from %2 to %3").arg(u).arg(ET).arg(MT)  ;
          plan -> ProgressText ( ProgressId [ 102 ] , M )         ;
          if ( SC . Query ( Q ) )                                 {
            M = tr("Remove %1 from %2").arg(u).arg(ET)            ;
            plan -> ProgressText ( ProgressId [ 102 ] , M )       ;
            Q = QString("delete from %1 where uuid = %2").arg(ET).arg(u) ;
            SC . Query ( Q )                                      ;
          }                                                       ;
        }                                                         ;
      }                                                           ;
    } while ( go && keep )                                        ;
    ///////////////////////////////////////////////////////////////
    go      = true                                                ;
    total   = Thumbs.count()                                      ;
    target  = 0                                                   ;
    vacancy = 0                                                   ;
    index   = 0                                                   ;
    ///////////////////////////////////////////////////////////////
    do                                                            {
      ET       = Thumbs [ total-1 ]                               ;
      remnants = SC.count(ET)                                     ;
      keep     = ( remnants > 0 )                                 ;
      if ( target < ( total - 1 ) )                               {
        bool locating = true                                      ;
        do                                                        {
          QString T = Thumbs[target]                              ;
          int     t = SC.count(T)                                 ;
          if (t==EachDepot)                                       {
            target++                                              ;
            if ( target >= ( total - 1) ) locating = false        ;
          } else                                                  {
            vacancy  = EachDepot - t                              ;
            if (vacancy>0) locating = false                       ;
          }                                                       ;
        } while (locating)                                        ;
        if (vacancy<=0) keep = false                              ;
      } else keep = false                                         ;
      if ( keep )                                                 {
        QString MT     = Thumbs[target]                           ;
        int     moving = remnants                                 ;
        UUIDs   Uuids                                             ;
        if (moving>vacancy) moving = vacancy                      ;
        plan -> Start    ( ProgressId [ 102 ] , &index , &go    ) ;
        plan -> setRange ( ProgressId [ 102 ] , 0      , moving ) ;
        Uuids  = SC.Uuids(ET,"uuid",SC.sql.Limit(0,moving))       ;
        index  = 0                                                ;
        moving = Uuids.count()                                    ;
        while (go && index<moving)                                {
          QString M                                               ;
          SUID    u = Uuids[index]                                ;
          index++                                                 ;
          Q = QString("insert into %1 (id,uuid,thumb,ltime) "
                      "select id,uuid,thumb,ltime from %2 where uuid = %3 ;")
              .arg(MT).arg(ET).arg(u)                             ;
          M = tr("Move %1 from %2 to %3").arg(u).arg(ET).arg(MT)  ;
          plan -> ProgressText ( ProgressId [ 102 ] , M )         ;
          if ( SC . Query ( Q ) )                                 {
            M = tr("Remove %1 from %2").arg(u).arg(ET)            ;
            plan -> ProgressText ( ProgressId [ 102 ] , M )       ;
            Q = QString("delete from %1 where uuid = %2").arg(ET).arg(u) ;
            SC . Query ( Q )                                      ;
          }                                                       ;
        }                                                         ;
      }                                                           ;
    } while ( go && keep )                                        ;
    ///////////////////////////////////////////////////////////////
    SC . close ( )                                                ;
    Alert  ( Done       )                                         ;
  } else                                                          {
    SC . close ( )                                                ;
    Alert  ( Error      )                                         ;
  }                                                               ;
  SC . remove ( )                                                 ;
  plan -> Finish ( ProgressId [ 102 ] )                           ;
}

void N::PictureDepot::Optimize(void)
{
  SqlConnection SC ( Depot )                                     ;
  if (SC.open("PictureDepot","Optimize"))                        {
    QString     Q                                                ;
    QStringList T                                                ;
    qint64      index = 0                                        ;
    bool        go    = true                                     ;
    T << Pictures                                                ;
    T << Thumbs                                                  ;
    plan -> Start    ( ProgressId [ 103 ] , &index , &go       ) ;
    plan -> setRange ( ProgressId [ 103 ] , 0      , T.count() ) ;
    while (go && index<T.count())                                {
      Q = QString("optimize table %1 ;").arg(T[index])           ;
      index++                                                    ;
      plan -> ProgressText ( ProgressId [ 103 ] , Q )            ;
      SC    . Query        ( Q                      )            ;
      Time :: msleep       ( 1000                   )            ;
    }                                                            ;
    SC . close ( )                                               ;
    Alert  ( Done       )                                        ;
  } else                                                         {
    SC . close ( )                                               ;
    Alert  ( Error      )                                        ;
  }                                                              ;
  SC . remove ( )                                                ;
  plan -> Finish   ( ProgressId [ 103 ] )                        ;
}
