#include <qtphotography.h>

#define hasMenu   isFunction(InterfaceHasMenu)
#define ALERT(ID) AnotherALERT(N::Command::ID)

N::PictureView:: PictureView    ( QWidget * parent , Plan * p         )
               : SnapView       (           parent ,        p         )
               , PictureManager (                           p         )
               , Group          ( 0 , Types::Gallery , Types::Picture )
               , dropAction     ( false                               )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PictureView::~PictureView (void)
{
}

void N::PictureView::Configure(void)
{
  StackSize          = 0                                                     ;
  Reservation        = false                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  LimitValues [  0 ] =  0                                                    ;
  LimitValues [  1 ] = 72                                                    ;
  LimitValues [  2 ] =  0                                                    ;
  LimitValues [  9 ] = 40                                                    ;
  LimitValues [ 11 ] = (int)Qt::AscendingOrder                               ;
  LimitValues [ 75 ] = 0                                                     ;
  LimitValues [ 76 ] = 100                                                   ;
  LimitValues [ 77 ] = 500                                                   ;
  ////////////////////////////////////////////////////////////////////////////
  LocalMsgs [ PixMsgFocusIn      ] = tr ( "Picture viewer obtains window focus"  ) ;
  LocalMsgs [ PixMsgColors       ] = tr ( "%1 x %2\n%3 colors"                   ) ;
  LocalMsgs [ PixMsgLocate       ] = tr ( "Total %1 pictures, displayed %2 start at %3" ) ;
  LocalMsgs [ PixMsgJoinS        ] = tr ( "%1 pictures join gallery %2"          ) ;
  LocalMsgs [ PixMsgJoinF        ] = tr ( "%1 pictures join gallery %2 finished" ) ;
  LocalMsgs [ PixMsgProgress     ] = tr ( "%v/%m pictures"                       ) ;
  LocalMsgs [ PixMsgImport       ] = tr ( "Importing pictures"                   ) ;
  LocalMsgs [ PixMsgAppended     ] = tr ( "Append %1 pictures into database"     ) ;
  LocalMsgs [ PixMsgExporting    ] = tr ( "Exporting pictures"                   ) ;
  LocalMsgs [ PixMsgExported     ] = tr ( "%1 pictures exported"                 ) ;
  LocalMsgs [ PixMsgSaveTo       ] = tr ( "Save pictures to directory"           ) ;
  LocalMsgs [ PixMsgFind         ] = tr ( "Find picture by internet site names"  ) ;
  LocalMsgs [ PixMsgKeyword      ] = tr ( "Keywords : "                          ) ;
  LocalMsgs [ PixMsgSites        ] = tr ( "Find picture sites"                   ) ;
  LocalMsgs [ PixMsgOrganize     ] = tr ( "Organize picture orders"              ) ;
  LocalMsgs [ PixMsgOrders       ] = tr ( "Ordering pictures"                    ) ;
  LocalMsgs [ PixMsgToDB         ] = tr ( "Append %1 pictures into database"     ) ;
  LocalMsgs [ PixMsgSelected     ] = tr ( "%1 pictures selected"                 ) ;
  LocalMsgs [ PixMsgMoving       ] = tr ( "Start moving %1 pictures"             ) ;
  LocalMsgs [ PixMsgImportImages ] = tr ( "Import [%1 x %2] image"               ) ;
  LocalMsgs [ PixMsgImportFiles  ] = tr ( "Import %1 files"                      ) ;
  LocalMsgs [ PixMsgAssignText   ] = tr ( "Assign [%1] to %2"                    ) ;
  LocalMsgs [ PixMsgAssignHtml   ] = tr ( "Assign HTML to %1"                    ) ;
  ////////////////////////////////////////////////////////////////////////////
  LocalIcons [ "Update" ] = QIcon ( ":/images/update.png"         )          ;
  LocalIcons [ "Import" ] = QIcon ( ":/images/importdatabase.png" )          ;
  LocalIcons [ "Tag"    ] = QIcon ( ":/images/tag.png"            )          ;
  ////////////////////////////////////////////////////////////////////////////
  dockingOrientation = 0                                                     ;
  dockingPlace       = Qt::BottomDockWidgetArea                              ;
  ////////////////////////////////////////////////////////////////////////////
  setAccessibleName ( "N::PictureView"        )                              ;
  setObjectName     ( "N::PictureView"        )                              ;
  setDropFlag       ( DropText     , true     )                              ;
  setDropFlag       ( DropUrls     , true     )                              ;
  setDropFlag       ( DropImage    , true     )                              ;
  setDropFlag       ( DropHtml     , true     )                              ;
  setDropFlag       ( DropColor    , false    )                              ;
  setDropFlag       ( DropTag      , true     )                              ;
  setDropFlag       ( DropPicture  , true     )                              ;
  setDropFlag       ( DropPeople   , false    )                              ;
  setDropFlag       ( DropVideo    , false    )                              ;
  setDropFlag       ( DropAlbum    , false    )                              ;
  setDropFlag       ( DropGender   , false    )                              ;
  setDropFlag       ( DropDivision , false    )                              ;
  ////////////////////////////////////////////////////////////////////////////
  setFunction       ( 1001         , true     )                              ;
  setFunction       ( 1002         , false    )                              ;
  setFunction       ( 121391000    , true     )                              ;
  ////////////////////////////////////////////////////////////////////////////
  setGridSize       ( QSize ( 136 , 136 )     )                              ;
  setMinimumSize    ( QSize ( 144 , 144 )     )                              ;
  ////////////////////////////////////////////////////////////////////////////
  plan -> setFont   ( this                    )                              ;
}

////////////////////////////////////////////////////////////////////
//                     System message handlers                    //
////////////////////////////////////////////////////////////////////

void N::PictureView::closeEvent(QCloseEvent * e)
{
  StopIcons                (   ) ;
  ListWidget :: closeEvent ( e ) ;
}

QMimeData * N::PictureView::dragMime(void)
{
  PickedUuids . clear ( )                                         ;
  if ( Items . count ( ) <= 0 ) return NULL                       ;
  QMimeData       * mime    = new QMimeData ( )                   ;
  QListWidgetItem * current = currentItem   ( )                   ;
  if ( Items . count ( ) == 1 )                                   {
    SUID        type = nListUuid(Items[0])                        ;
    QByteArray  data = CreateByteArray(type)                      ;
    mime -> setData ("picture/uuid",data)                         ;
  } else                                                          {
    UUIDs       Uuids = Selected        (       )                 ;
    QByteArray  data  = CreateByteArray ( Uuids )                 ;
    mime -> setData ("picture/uuids",data)                        ;
    PickedUuids = Uuids                                           ;
  }                                                               ;
  if ( NotNull ( current ) )                                      {
    QIcon  icon  = current -> icon ( )                            ;
    if (!icon.isNull())                                           {
      QSize  s     = iconSize ( )                                 ;
      QImage image = icon . pixmap ( s ) . toImage ( )            ;
      if (!image.isNull())                                        {
        mime -> setImageData(image)                               ;
      }                                                           ;
    }                                                             ;
  }                                                               ;
  QString tooltip                                                 ;
  tooltip = QString(LocalMsgs[PixMsgSelected]).arg(Items.count()) ;
  QToolTip :: showText ( QCursor::pos ( ) , tooltip )             ;
  return mime                                                     ;
}

QString N::PictureView::MimeType(const QMimeData * mime)
{
  return AbstractGui :: MimeType       (
           mime                        ,
           "picture/uuid;picture/uuids;"
           "tag/uuid;tag/uuids"      ) ;
}

UUIDs N::PictureView::MimeUuids(const QMimeData * mime,QString mimetype)
{
  UUIDs      Uuids                            ;
  QByteArray data = mime -> data ( mimetype ) ;
  if ( data . size ( ) <= 0 ) return Uuids    ;
  if ( mimetype == "picture/uuid"  )          {
    Uuids << GetUuid  ( data )                ;
  } else
  if ( mimetype == "picture/uuids" )          {
    Uuids  = GetUuids ( data )                ;
  } else
  if ( mimetype == "tag/uuid"      )          {
    Uuids << GetUuid(data)                    ;
  } else
  if ( mimetype == "tag/uuids"     )          {
    Uuids  = GetUuids ( data )                ;
  }                                           ;
  return Uuids                                ;
}

bool N::PictureView::allowMoving(void)
{
  if ( isFirst  ( ) ) return true ;
  if ( isSecond ( ) ) return true ;
  return false                    ;
}

bool N::PictureView::acceptDrop(QWidget * source,const QMimeData * mime)
{
  return dropHandler ( mime ) ;
}

bool N::PictureView::MovingItems(QWidget * source,const QMimeData * mime,QPoint pos)
{
  if ( ! allowMoving ( ) ) return false                                      ;
  QString mtype                                                              ;
  UUIDs   Uuids                                                              ;
  QString m                                                                  ;
  mtype = MimeType  ( mime         )                                         ;
  Uuids = MimeUuids ( mime , mtype )                                         ;
  m     = QString ( LocalMsgs [ PixMsgMoving ] ) . arg ( Uuids . count ( ) ) ;
  plan -> showMessage   ( m )                                                ;
  plan -> processEvents (   )                                                ;
  return true                                                                ;
}

bool N::PictureView::ImportImages(QWidget * source,const QMimeData * mime,QPoint pos)
{
  QImage image = qvariant_cast<QImage> ( mime -> imageData ( ) ) ;
  if ( image . isNull ( ) ) return false                         ;
  ////////////////////////////////////////////////////////////////
  QString m                                                      ;
  m = QString ( LocalMsgs [ PixMsgImportImages ]                 )
      . arg   ( image . width  ( )                               )
      . arg   ( image . height ( )                             ) ;
  plan -> showMessage   ( m )                                    ;
  plan -> processEvents (   )                                    ;
  return true                                                    ;
}

bool N::PictureView::ImportFiles(QWidget * source,const QMimeData * mime,QPoint pos)
{
  QList<QUrl> urls = mime -> urls ( )           ;
  if ( urls . count ( ) <= 0 ) return false     ;
  QString m                                     ;
  m = QString ( LocalMsgs [ PixMsgImportFiles ] )
      . arg   ( urls . count ( )              ) ;
  plan -> showMessage   ( m )                   ;
  plan -> processEvents (   )                   ;
  return true                                   ;
}

bool N::PictureView::PictureObjects(QWidget * source,const QMimeData * mime,QPoint pos)
{
  QString mtype                                                      ;
  UUIDs   Uuids                                                      ;
  mtype = MimeType  ( mime         )                                 ;
  Uuids = MimeUuids ( mime , mtype )                                 ;
  ////////////////////////////////////////////////////////////////////
  if ( ( "picture/uuid" == mtype ) || ( "picture/uuids" == mtype ) ) {
    plan -> showMessage                                              (
      tr    ( "Copy %1 pictures from %2"                             )
      . arg ( Uuids . count ( )                                      )
      . arg ( source -> windowTitle ( ) )                          ) ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  if ( ( "tag/uuid"     == mtype ) || ( "tag/uuids"     == mtype ) ) {
    plan -> showMessage                                              (
      tr    ( "Assign %1 tags to %2"                                 )
      . arg ( Uuids . count ( )                                      )
      . arg ( source -> windowTitle ( ) )                          ) ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  plan -> processEvents ( )                                          ;
  ////////////////////////////////////////////////////////////////////
  return true                                                        ;
}

bool N::PictureView::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  if                      ( source == this       ) {
    return MovingItems    ( source , mime , pos  ) ;
  }                                                ;
  //////////////////////////////////////////////////
  if                      ( mime -> hasImage ( ) ) {
    return ImportImages   ( source , mime , pos  ) ;
  }                                                ;
  //////////////////////////////////////////////////
  if                      ( mime -> hasUrls ( )  ) {
    return ImportFiles    ( source , mime , pos  ) ;
  }                                                ;
  //////////////////////////////////////////////////
  return   PictureObjects ( source , mime , pos  ) ;
}

bool N::PictureView::dropMoving(QWidget * source,const QMimeData * mime,QPoint pos)
{
  if ( dropAction                                       ) return false ;
  if   ( source == this                                 )              {
    if ( ! allowMoving ( )                              ) return false ;
    QListWidgetItem * atItem = itemAt ( pos )                          ;
    if ( IsNull  ( atItem )                             ) return false ;
    if ( NotNull ( atItem ) && atItem -> isSelected ( ) ) return false ;
  }                                                                    ;
  return true                                                          ;
}

bool N::PictureView::dropAppend(QWidget * source,const QMimeData * mime,QPoint pos)
{
  if ( dropAction ) return false           ;
  return dropItems ( source , mime , pos ) ;
}

////////////////////////////////////////////////////////////////////
//                   Picture information handlers                 //
////////////////////////////////////////////////////////////////////

void N::PictureView::StopIcons(void)
{
  stopForcely                        (      ) ;
  while ( detection ( ) ) Time::skip ( 100  ) ;
  emit Leave                         ( this ) ;
}

bool N::PictureView::isOwner(void)
{
  if ( isFirst  ( ) ) return true  ;
  if ( isSecond ( ) ) return true  ;
  return                     false ;
}

int N::PictureView::Total(void)
{
  int total = 0                                          ;
  EnterSQL                          ( SC , plan -> sql ) ;
    total = PictureManager :: Count ( SC               ) ;
  LeaveSQL                          ( SC , plan -> sql ) ;
  return total                                           ;
}

int N::PictureView::Total(SUID Gallery)
{
  int total = 0                     ;
  EnterSQL ( SC , plan -> sql )     ;
    total = PictureManager :: Count (
              SC                    ,
              Gallery               ,
              t1                    ,
              relation            ) ;
  LeaveSQL ( SC , plan -> sql )     ;
  return total                      ;
}

int N::PictureView::TotalOwners(SUID u,int T2)
{
  int total = 0                                                 ;
  GroupItems GI (      plan        )                            ;
  EnterSQL      ( SC , plan -> sql )                            ;
    QString Q                                                   ;
    GI . AutoMap    = true                                      ;
    GI . GroupTable = GI . LookTable ( t1 , T2 , relation )     ;
    Q               = SC . sql . SelectFrom                     (
                        "count(*)"                              ,
                        GI . GroupTable                         ,
                        SecondItem ( u , t1 , T2 , relation ) ) ;
    if ( SC . Fetch ( Q ) ) total = SC . Int ( 0 )              ;
  LeaveSQL      ( SC , plan -> sql )                            ;
  return total                                                  ;
}

UUIDs N::PictureView::Pictures(int StartId,int Size)
{
  UUIDs Uuids                                                   ;
  Items . clear ( )                                             ;
  EnterSQL ( SC , plan->sql )                                   ;
    Uuids = SC . Uuids                                          (
              PlanTable(PictureOrders)                          ,
              "uuid"                                            ,
              QString ( "%1 %2"                                 )
              . arg   ( SC . OrderByAsc("id"                  ) )
              . arg   ( SC . sql . Limit ( StartId , Size ) ) ) ;
  LeaveSQL ( SC , plan->sql )                                   ;
  return Uuids                                                  ;
}

UUIDs N::PictureView::Pictures(SUID Gallery,int StartId,int Size)
{
  UUIDs Uuids                                       ;
  Items . clear ( )                                 ;
  EnterSQL ( SC , plan->sql )                       ;
    Uuids = Subordination                           (
              SC                                    ,
              Gallery                               ,
              t1                                    ,
              t2                                    ,
              relation                              ,
              SC . OrderByAsc  ( "position"       ) ,
              SC . sql . Limit ( StartId , Size ) ) ;
  LeaveSQL ( SC , plan->sql )                       ;
  return Uuids                                      ;
}

UUIDs N::PictureView::PictureOwners(SUID u,int T2,int StartId,int Size)
{
  UUIDs Uuids                                       ;
  Items . clear ( )                                 ;
  EnterSQL ( SC , plan->sql )                       ;
    Uuids = GetOwners                               (
              SC                                    ,
              u                                     ,
              Types::Picture                        ,
              T2                                    ,
              relation                              ,
              SC . OrderByAsc  ( "id"             ) ,
              SC . sql . Limit ( StartId , Size ) ) ;
  LeaveSQL ( SC , plan->sql )                       ;
  return Uuids                                      ;
}

void N::PictureView::FetchUUIDs(void)
{
  UUIDs Uuids                                              ;
  int   startId = LimitValues [ 0 ]                        ;
  int   page    = LimitValues [ 1 ]                        ;
  int   total   = LimitValues [ 2 ]                        ;
  if ( page <= 0 ) page = total                            ;
  if                      ( isFirst  ( )                 ) {
    Uuids = Pictures      ( first  ,      startId , page ) ;
  } else
  if                      ( isSecond ( )                 ) {
    Uuids = PictureOwners ( second , t2 , startId , page ) ;
  } else                                                   {
    Uuids = Pictures      (               startId , page ) ;
  }                                                        ;
  UuidMaps [ 10001 ] = Uuids                               ;
}

void N::PictureView::FetchTOTALs(void)
{
  int total = 0                            ;
  if                      ( isFirst  ( ) ) {
    total   = Total       ( first        ) ;
  } else
  if                      ( isSecond ( ) ) {
    total   = TotalOwners ( second , t2  ) ;
  } else                                   {
    total   = Total       (              ) ;
  }                                        ;
  LimitValues [ 2 ] = total                ;
}

bool N::PictureView::PictureDetails (SqlConnection & SC,QListWidgetItem * item)
{
  nKickOut           ( IsNull(item) , false )      ;
  SUID u = nListUuid (        item          )      ;
  nKickOut           ( ( u <= 0 )   , false )      ;
  //////////////////////////////////////////////////
  QString tooltip = ""                             ;
  int     colors = 0                               ;
  QSize   gsize                                    ;
  gsize  = PictureManager :: Geometry   ( SC , u ) ;
  colors = PictureManager :: ColorCount ( SC , u ) ;
  tooltip = QString ( LocalMsgs [ PixMsgColors ]   )
            . arg   ( gsize . width  ( )           )
            . arg   ( gsize . height ( )           )
            . arg   ( colors                     ) ;
  item   -> setToolTip ( tooltip )                 ;
  //////////////////////////////////////////////////
  return true                                      ;
}

bool N::PictureView::PictureId(SqlConnection & SC,QListWidgetItem * item)
{
  nKickOut           ( IsNull(item) , false ) ;
  SUID u = nListUuid (        item          ) ;
  nKickOut           ( ( u <= 0 )   , false ) ;
  /////////////////////////////////////////////
  QString Q                                   ;
  Q = SC . sql . SelectFrom                   (
        SC . Columns   ( 1 , "id" )           ,
        PlanTable(PictureOrders)              ,
        SC . WhereUuid ( u        )         ) ;
  if ( SC . Fetch ( Q ) )                     {
    int id = SC . Int ( 0 )                   ;
    Q = QString("%1 [%2]").arg(u).arg(id)     ;
    item -> setToolTip ( Q )                  ;
    return true                               ;
  }                                           ;
  /////////////////////////////////////////////
  return false                                ;
}

bool N::PictureView::PictureURLs(SqlConnection & SC,QListWidgetItem * item)
{
  nKickOut           ( IsNull(item) , false ) ;
  SUID u = nListUuid (        item          ) ;
  nKickOut           ( ( u <= 0 )   , false ) ;
  /////////////////////////////////////////////
  UUIDs Puids                                 ;
  Puids = Subordination                       (
            SC                                ,
            u                                 ,
            Types::Picture                    ,
            Types::URL                        ,
            Groups::Subordination             ,
            SC.OrderByAsc("position")       ) ;
  if ( Puids.count() <= 0 ) return false      ;
  /////////////////////////////////////////////
  SUID           puid                         ;
  QStringList    L                            ;
  NetworkManager NM  ( plan                 ) ;
  /////////////////////////////////////////////
  foreach ( puid , Puids )                    {
    QUrl u = NM . PageUrl ( SC , puid )       ;
    if (u.isValid()) L << u.toString()        ;
  }                                           ;
  /////////////////////////////////////////////
  if (L.count()>0)                            {
    QString M                                 ;
    L     . prepend    ( ""                 ) ;
    L     . prepend    ( QString::number(u) ) ;
    M     = L . join   ( "\n"               ) ;
    item -> setToolTip ( M                  ) ;
    return true                               ;
  }                                           ;
  /////////////////////////////////////////////
  return false                                ;
}

bool N::PictureView::ReportDetails(SqlConnection & SC,QListWidgetItem * item,int flags)
{
  switch                           ( flags     ) {
    case 0 : return PictureDetails ( SC , item ) ;
    case 1 : return PictureId      ( SC , item ) ;
    case 2 : return PictureURLs    ( SC , item ) ;
  }                                              ;
  return false                                   ;
}

////////////////////////////////////////////////////////////////////
//                           Gui handlers                         //
////////////////////////////////////////////////////////////////////

bool N::PictureView::FocusIn(void)
{
  nKickOut          ( IsNull ( plan ) , false               ) ;
  DisableAllActions (                                       ) ;
  AssignAction      ( Label           , windowTitle ( )     ) ;
  LinkAction        ( Delete          , Delete      ( )     ) ;
  LinkAction        ( Font            , setFont     ( )     ) ;
  LinkAction        ( Save            , Save        ( )     ) ;
  LinkAction        ( Import          , Import      ( )     ) ;
  LinkAction        ( GoHome          , Home        ( )     ) ;
  LinkAction        ( GoEnd           , End         ( )     ) ;
  LinkAction        ( PreviousPage    , Previous    ( )     ) ;
  LinkAction        ( NextPage        , Next        ( )     ) ;
  if                ( isOwner ( )                           ) {
    LinkAction      ( SaveAll         , SaveAll     ( )     ) ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  alert             ( "Click" , LocalMsgs [ PixMsgFocusIn ] ) ;
  return true                                                 ;
}

bool N::PictureView::Relocation(void)
{
  SnapView :: Relocation ( ) ;
  return false               ;
}

void N::PictureView::Home(void)
{
  LimitValues [ 0 ] = 0 ;
  Reload ( )            ;
}

void N::PictureView::Previous(void)
{
  int Start = LimitValues [ 0 ] ;
  int Page  = LimitValues [ 1 ] ;
  Start -= Page                 ;
  if (Start<0) Start = 0        ;
  LimitValues [ 0 ] = Start     ;
  Reload ( )                    ;
}

void N::PictureView::Next(void)
{
  int Start = LimitValues [ 0 ]  ;
  int Page  = LimitValues [ 1 ]  ;
  int Total = LimitValues [ 2 ]  ;
  Start += Page                  ;
  Total -= Page                  ;
  if (Start>Total) Start = Total ;
  LimitValues [ 0 ] = Start      ;
  Reload ( )                     ;
}

void N::PictureView::End(void)
{
  int Start = LimitValues [ 0 ] ;
  int Page  = LimitValues [ 1 ] ;
  int Total = LimitValues [ 2 ] ;
  Start = Total - Page          ;
  if (Start<0) Start = 0        ;
  LimitValues [ 0 ] = Start     ;
  Reload ( )                    ;
}

////////////////////////////////////////////////////////////////////
//                     Picture object handlers                    //
////////////////////////////////////////////////////////////////////

void N::PictureView::View(UUIDs & Uuids)
{
  emit clearItems       (                ) ;
  emit assignEnabling   ( false          ) ;
  emit assignWaitCursor (                ) ;
  plan -> processEvents (                ) ;
  LockGui               (                ) ;
  blockSignals          ( true           ) ;
  //////////////////////////////////////////
  int             PPL = LimitValues [ 76 ] ;
  int             PRC = LimitValues [ 77 ] ;
  LoopInt         LIP ( 0 , 0 , PPL )      ;
  QIcon           ICON                     ;
  ListWidgetItems ITEMs                    ;
  bool            GUI                      ;
  //////////////////////////////////////////
  ICON  = EmptyIcon ( iconSize() )         ;
  ITEMs = New       ( Uuids      )         ;
  GUI   = ( ITEMs.count() > PRC  )         ;
  for (int i=0;i<ITEMs.count();i++)        {
    ITEMs [ i ] -> setIcon ( ICON        ) ;
    QListWidget :: addItem ( ITEMs [ i ] ) ;
    if ( GUI )                             {
      ++LIP                                ;
      if ( 0 == LIP.V )                    {
        plan -> processEvents ( )          ;
      }                                    ;
    }                                      ;
  }                                        ;
  //////////////////////////////////////////
  blockSignals           ( false         ) ;
  UnlockGui              (               ) ;
  emit assignArrowCursor (               ) ;
  emit assignEnabling    ( true          ) ;
  SnapView::Relocation   (               ) ;
  //////////////////////////////////////////
  QString TT                               ;
  QString MM                               ;
  LimitValues [ 75 ] ++                    ;
  TT = LocalMsgs [ PixMsgLocate ]          ;
  MM = QString ( TT                        )
      . arg    ( LimitValues   [ 2 ]       )
      . arg    ( ITEMs . count (   )       )
      . arg    ( LimitValues   [ 0 ]     ) ;
  plan -> processEvents (                ) ;
  emit assignToolTip    ( MM             ) ;
  //////////////////////////////////////////
  VarArgs V                                ;
  V << 30002                               ;
  V << 0                                   ;
  addSequence           ( V              ) ;
  addSequence           ( 30001          ) ;
  StartCommand          (                ) ;
  plan -> processEvents (                ) ;
}

void N::PictureView::MoveToEnd(UUIDs & Uuids)
{
  AbstractGui::Mutex.lock()                             ;
  emit assignEnabling ( false )                         ;
  SqlConnection Connection(plan->sql)                   ;
  if ( Connection . open ( FunctionString ) )           {
    int final = 0                                       ;
    CUIDs IDs                                           ;
    QString Q                                           ;
    Q = Connection.sql.SelectFrom                       (
          "id"                                          ,
          PlanTable(PictureOrders)                      ,
          "order by id desc limit 0,1"                ) ;
    if (Connection.Query(Q) && Connection.Next())       {
      final = Connection.Value(0).toInt()               ;
    }                                                   ;
    final++                                             ;
    for (int i=0;i<Uuids.count();i++)                   {
      SUID uuid = Uuids[i]                              ;
      Q = Connection.sql.SelectFrom                     (
          "id"                                          ,
          PlanTable(PictureOrders)                      ,
          Connection.WhereUuid(uuid)                  ) ;
      if (Connection.Query(Q) && Connection.Next())     {
        IDs << Connection.Value(0).toInt()              ;
      }                                                 ;
    }                                                   ;
    Connection.transaction()                            ;
    for (int i=0;i<IDs.count();i++,final++)             {
      Q = QString("update %1 set id = %2 where id = %3;")
         .arg(plan->Tables[Tables::PictureOrders])
         .arg(final).arg(IDs[i])                        ;
      Connection.Query(Q)                               ;
    }                                                   ;
    Connection.commit()                                 ;
    Connection.close()                                  ;
  }                                                     ;
  Connection.remove()                                   ;
  emit assignEnabling ( true )                          ;
  AbstractGui::Mutex.unlock()                           ;
}

void N::PictureView::MoveToEnd(SUID Gallery,UUIDs & Uuids)
{
  MoveToEnd ( Gallery , t1 , relation , Uuids ) ;
}

void N::PictureView::MoveToEnd(SUID objectUuid,int objectType,int objectRelation,UUIDs & Uuids)
{
  nDropOut ( Uuids.count() <= 0 )                       ;
  emit assignEnabling ( false )                         ;
  EnterSQL(Connection,plan->sql)                        ;
    int final = 0                                       ;
    QString Q                                           ;
    Q = Connection.sql.SelectFrom                       (
          "position"                                    ,
          PlanTable(Groups)                             ,
          QString("where first = %1 "
                  "and t1 = %2 "
                  "and t2 = %3 "
                  "and relation = %4 "
                  "order by position desc limit 0,1"    )
            .arg(objectUuid                             )
            .arg(objectType                             )
            .arg(Types::Picture               )
            .arg(objectRelation                         )
        )                                               ;
    if (Connection.Fetch(Q)) final = Connection.Int(0)  ;
    final++                                             ;
    Connection.transaction()                            ;
    for (int i=0;i<Uuids.count();i++,final++)           {
      SUID uuid = Uuids[i]                              ;
      Q = Connection.sql.Update(PlanTable(Groups))      +
          QString("position = %1"
                  " where first = %2"
                  " and second = %3"
                  " and t1 = %4"
                  " and t2 = %5"
                  " and relation = %6 ;"                )
              .arg(final                                )
              .arg(objectUuid                           )
              .arg(uuid                                 )
              .arg(objectType                           )
              .arg(Types::Picture             )
              .arg(objectRelation                     ) ;
      Connection.Query(Q)                               ;
      if ((i%1000)==999)                                {
        Connection . commit      ( )                    ;
        Connection . transaction ( )                    ;
      }                                                 ;
    }                                                   ;
    Connection.commit()                                 ;
  LeaveSQL(Connection,plan->sql)                        ;
  emit assignEnabling ( true )                          ;
}

void N::PictureView::Relocate(SUID objectUuid,int objectType,int objectRelation)
{
  emit assignEnabling ( false                    ) ;
  EnterSQL            ( Connection , plan -> sql ) ;
    GroupItems::Relocate                           (
      Connection                                   ,
      objectUuid                                   ,
      objectType                                   ,
      Types::Picture                               ,
      objectRelation                             ) ;
  LeaveSQL            ( Connection , plan -> sql ) ;
  emit assignEnabling ( true                     ) ;
  ALERT               ( Done                     ) ;
}

void N::PictureView::Join(SUID Gallery,SUID Before,UUIDs & Uuids)
{
  QString M                                              ;
  QString T                                              ;
  ////////////////////////////////////////////////////////
  T = LocalMsgs [ PixMsgJoinS ]                          ;
  M = QString(T).arg(Uuids.count()).arg(windowTitle())   ;
  plan -> showMessage   ( M )                            ;
  plan -> processEvents (   )                            ;
  ////////////////////////////////////////////////////////
  QString fsg                                            ;
  UUIDs   U   = Uuids                                    ;
  VarArgs V                                              ;
  Group   G                                              ;
  T   = LocalMsgs [ PixMsgJoinF ]                        ;
  fsg = QString(T).arg(Uuids.count()).arg(windowTitle()) ;
  ////////////////////////////////////////////////////////
  G  . first      = Gallery                              ;
  G  . second     = 0                                    ;
  G  . t1         = t1                                   ;
  G  . t2         = t2                                   ;
  G  . relation   = relation                             ;
  G  . position   = Before                               ;
  G  . membership = 1.0                                  ;
  V << 10013                                             ;
  V << fsg                                               ;
  G  . addParaments ( V     )                            ;
  V << U . count    (       )                            ;
  toVariants        ( U , V )                            ;
  addSequence       ( V     )                            ;
  ////////////////////////////////////////////////////////
  dropAction = false                                     ;
  emit TriggerCommand ( )                                ;
}

bool N::PictureView::startup(void)
{
  return Reload ( ) ;
}

bool N::PictureView::Reload(bool maximize)
{
  if ( isLoading ( ) )      {
    return false            ;
  }                         ;
  emit clearItems ( )       ;
  VarArgs V                 ;
  V << 10002                ;
  V << maximize             ;
  addSequence         ( V ) ;
  emit TriggerCommand (   ) ;
  return true               ;
}

bool N::PictureView::Delete(SqlConnection & Connection,SUID u,int Type)
{
  QString Q                                                             ;
  UUIDs   Uuids = Selected ( )                                          ;
  SUID    z                                                             ;
  GroupItems GI ( plan )                                                ;
  GI . AutoMap    = true                                                ;
  GI . GroupTable = GI . LookTable ( Type , Types::Picture , relation ) ;
  foreach ( z , Uuids )                                                 {
    Q = Connection . sql . DeleteFrom                                   (
          GI.GroupTable                                                 ,
          ExactItem                                                     (
            u                                                           ,
            z                                                           ,
            Type                                                        ,
            Types::Picture                                              ,
            relation                                                ) ) ;
      Connection . Query ( Q )                                          ;
      takeItem ( row ( uuidAt ( z ) ) )                                 ;
  }                                                                     ;
  return true                                                           ;
}

void N::PictureView::DeleteByThread(ThreadData * d)
{ Q_UNUSED ( d                 ) ;
  EnterSQL ( SC , plan -> sql  ) ;
    Delete ( SC , first , t1   ) ;
  LeaveSQL ( SC , plan -> sql  ) ;
  ALERT    ( Done              ) ;
}

void N::PictureView::Delete(void)
{
  if ( !isOwner ( ) ) return ;
  start ( 40005 )            ;
}

void N::PictureView::DeleteThis(void)
{
  if ( !isOwner() ) return         ;
  //////////////////////////////////
  QListWidgetItem * it             ;
  it = currentItem ( )             ;
  nDropOut ( IsNull(it)     )      ;
  SUID u = nListUuid(it)           ;
  nDropOut ( u <= 0         )      ;
  EnterSQL ( SC , plan->sql )      ;
    bool correct                   ;
    correct = ExactDetach          (
                SC                 ,
                first              ,
                u                  ,
                t1                 ,
                t2                 ,
                relation         ) ;
    if (correct) takeItem(row(it)) ;
  LeaveSQL ( SC , plan->sql )      ;
  ALERT    ( Done           )      ;
}

void N::PictureView::MoveIndexToEnd(void)
{
  UUIDs Uuids = Selected ( )      ;
  if (isFirst())                  {
    MoveToEnd   ( first , Uuids ) ;
  } else
  if (isSecond())                 {
  } else                          {
    MoveToEnd   ( Uuids         ) ;
  }                               ;
  Reload        (               ) ;
  setSelections ( Uuids         ) ;
}

void N::PictureView::MoveAllToEnd(void)
{
  if ( !isOwner() ) return                              ;
  ///////////////////////////////////////////////////////
  LockGui             (       )                         ;
  emit assignEnabling ( false )                         ;
  SqlConnection Connection ( plan -> sql )              ;
  if ( Connection . open ( FunctionString ) )           {
    UUIDs Uuids                                         ;
    Uuids = Subordination                               (
              Connection                                ,
              first                                     ,
              t1                                        ,
              t2                                        ,
              relation                                  ,
              Connection.OrderByAsc("position")       ) ;
    /////////////////////////////////////////////////////
    int final = 0                                       ;
    CUIDs IDs                                           ;
    QString Q                                           ;
    Q = Connection.sql.SelectFrom                       (
          Connection.Columns(1,"id")                    ,
          PlanTable(PictureOrders)                      ,
          ""                                            ,
          Connection.OrderByDesc("id")                  ,
          Connection.sql.Limit(0,1)                   ) ;
    if (Connection.Query(Q) && Connection.Next())       {
      final = Connection.Value(0).toInt()               ;
    }                                                   ;
    final++                                             ;
    /////////////////////////////////////////////////////
    for (int i=0;i<Uuids.count();i++)                   {
      SUID u = Uuids[i]                                 ;
      Q = Connection.sql.SelectFrom                     (
          Connection.Columns(1,"id")                    ,
          PlanTable(PictureOrders)                      ,
          Connection.WhereUuid(u)                     ) ;
      if (Connection.Query(Q) && Connection.Next())     {
        IDs << Connection.Int(0)                        ;
      }                                                 ;
    }                                                   ;
    /////////////////////////////////////////////////////
    Connection.transaction()                            ;
    for (int i=0;i<IDs.count();i++,final++)             {
      Q = QString                                       (
            "update `%1` "
            "set `id` = %2 "
            "where `id` = %3;"                          )
         . arg ( PlanTable(PictureOrders)               )
         . arg ( final                                  )
         . arg ( IDs[i]                               ) ;
      Connection.Query(Q)                               ;
      if ((i%1000)==999)                                {
        Connection.commit()                             ;
        Connection.transaction()                        ;
      }                                                 ;
    }                                                   ;
    Connection.commit()                                 ;
    /////////////////////////////////////////////////////
    Connection.close()                                  ;
  }                                                     ;
  Connection.remove()                                   ;
  ///////////////////////////////////////////////////////
  emit assignEnabling ( true )                          ;
  UnlockGui           (      )                          ;
  ALERT               ( Done )                          ;
}

void N::PictureView::MoveInternal(SUID before,UUIDs Uuids)
{
  Reload ( ) ;
}

void N::PictureView::CopyPictures(SUID before,UUIDs Uuids)
{
  if   ( ! isOwner ( ) ) return   ;
  Join ( first , before , Uuids ) ;
}

void N::PictureView::ImportPictures(SUID before,QList<QUrl> urls)
{
  QStringList files                                             ;
  for (int i=0;i<urls.count();i++)                              {
    QString lf = urls[i].toLocalFile()                          ;
    if (lf.length()>0) files << lf                              ;
  }                                                             ;
  if ( files.count() <= 0 ) return                              ;
  ///////////////////////////////////////////////////////////////
  int    total = 0                                              ;
  qint64 cnt   = files . count ( )                              ;
  qint64 index = 0                                              ;
  int    id    = -1                                             ;
  bool   go    = true                                           ;
  id = plan -> Progress ( LocalMsgs [ PixMsgImport   ]          ,
                          LocalMsgs [ PixMsgProgress ]        ) ;
  plan -> Start    ( id , &index , &go )                        ;
  plan -> setRange ( id , 0      , cnt )                        ;
  ///////////////////////////////////////////////////////////////
  UUIDs Uuids                                                   ;
  EnterSQL       ( SC , plan->sql )                             ;
    SUID  uuid                                                  ;
    for (int i = 0 ; i < files . count ( ) ; i++ , index++ )    {
      plan -> showMessage   ( files [ i ] )                     ;
      plan -> processEvents (             )                     ;
      uuid = 0                                                  ;
      PictureManager :: Import ( SC , files[i] , uuid )         ;
      if ( uuid > 0 )                                           {
        if ( ! Uuids . contains ( uuid ) ) Uuids << uuid        ;
        total++                                                 ;
      }                                                         ;
    }                                                           ;
  LeaveSQL       ( SC , plan->sql )                             ;
  plan -> Finish ( id             )                             ;
  ///////////////////////////////////////////////////////////////
  if ( isOwner ( ) ) Join ( first , before , Uuids )            ;
  ///////////////////////////////////////////////////////////////
  QString m = QString ( LocalMsgs[PixMsgAppended] ).arg(total)  ;
  plan -> showMessage ( m )                                     ;
  plan -> Talk        ( m )                                     ;
}

void N::PictureView::JoinImage(const QMimeData * mime)
{
}

bool N::PictureView::dropText(QWidget * source,QPointF psf,const QString & text)
{
  if ( this == source ) return false                 ;
  QPoint pos = psf . toPoint ( )                     ;
  QListWidgetItem * atItem = itemAt ( pos )          ;
  SUID u = 0                                         ;
  if ( NotNull ( atItem ) ) u = nListUuid ( atItem ) ;
  VarArgs args                                       ;
  args << u                                          ;
  args << text                                       ;
  start ( 40006 , args )                             ;
  return true                                        ;
}

bool N::PictureView::dropUrls(QWidget * source,QPointF psf,const QList<QUrl> & urls)
{
  if ( this == source ) return false                ;
  QStringList files                                 ;
  QPoint pos = psf . toPoint ( )                    ;
  for (int i=0;i<urls.count();i++)                  {
    QString filename = urls [ i ] . toLocalFile ( ) ;
    QFileInfo FI ( filename )                       ;
    if ( FI . exists ( ) ) files << filename        ;
  }                                                 ;
  if ( files . count ( ) <= 0 ) return false        ;
  VarArgs     args                                  ;
  for (int i = 0 ; i < files . count ( ) ; i++ )    {
    args << files [ i ]                             ;
  }                                                 ;
  start ( 40007 , args )                            ;
  return true                                       ;
}

bool N::PictureView::dropImage (
        QWidget      * source  ,
        QPointF        psf     ,
        const QImage & image   )
{
  if ( source == this ) return true               ;
  QPoint pos = psf.toPoint()                      ;
  QListWidgetItem * atItem = itemAt(pos)          ;
  SUID atUuid = 0                                 ;
  if (NotNull(atItem))                            {
    atUuid = nListUuid(atItem)                    ;
  }                                               ;
  SUID puid    = 0                                ;
  bool success = false                            ;
  QImage I = image                                ;
  success = PictureManager :: Import ( I , puid ) ;
  if ( puid <= 0  ) return true                   ;
  if ( !isOwner() ) return true                   ;
  dropAction = true                               ;
  UUIDs Uuids                                     ;
  Uuids << puid                                   ;
  SqlConnection SC ( plan -> sql )                ;
  if ( SC . open ( FunctionString ) )             {
    GroupItems :: Join                            (
      SC                                          ,
      first                                       ,
      t1                                          ,
      t2                                          ,
      relation                                    ,
      atUuid                                      ,
      Uuids                                     ) ;
    SC . close  (      )                          ;
  }                                               ;
  SC   . remove (      )                          ;
  ALERT         ( Done )                          ;
  dropAction = false                              ;
  return true                                     ;
}

bool N::PictureView::dropHtml(QWidget * source,QPointF psf,const QString & html)
{
  if ( this == source ) return false                 ;
  QPoint pos = psf . toPoint ( )                     ;
  QListWidgetItem * atItem = itemAt ( pos )          ;
  SUID u = 0                                         ;
  if ( NotNull ( atItem ) ) u = nListUuid ( atItem ) ;
  VarArgs args                                       ;
  args << u                                          ;
  args << html                                       ;
  start ( 40008 , args )                             ;
  return true                                        ;
}

bool N::PictureView::dropTags(QWidget * source,QPointF psf,const UUIDs & Uuids)
{
  if ( Uuids . count () <= 0 ) return false ;
  if ( source == this        ) return false ;
  QPoint pos = psf . toPoint ( )            ;
  QListWidgetItem * atItem = itemAt ( pos ) ;
  if ( IsNull ( atItem )     ) return false ;
  SUID    puid = nListUuid(atItem)          ;
  VarArgs args                              ;
  args << puid                              ;
  toVariants ( Uuids , args )               ;
  start ( 40009 , args )                    ;
  return true                               ;
}

bool N::PictureView::dropPictures(QWidget * source,QPointF psf,const UUIDs & Uuids)
{
  QPoint pos = psf.toPoint()                   ;
  QListWidgetItem * atItem = itemAt(pos)       ;
  if ( source == this )                        {
    if (IsNull(atItem)) return true            ;
    SUID  auid  = nListUuid(atItem)            ;
    UUIDs Puids = Uuids                        ;
    MovePicture   ( auid , Uuids  )            ;
    Reload        (               )            ;
    setSelections ( Puids         )            ;
  } else                                       {
    if ( !isOwner() ) return true              ;
    UUIDs ITEMs = ItemUuids()                  ;
    UUIDs AUIDs                                ;
    SUID  u                                    ;
    SUID  a = 0                                ;
    if (NotNull(atItem)) a = nListUuid(atItem) ;
    foreach (u,Uuids)                          {
      if (!ITEMs.contains(u)) AUIDs << u       ;
    }                                          ;
    if (AUIDs.count()<=0) return true          ;
    CopyPictures ( a , AUIDs )                 ;
  }                                            ;
  return true                                  ;
}

void N::PictureView::MovePicture(SUID atUuid,const UUIDs & Uuids)
{
  dropAction = true                            ;
  SUID            uuid                         ;
  UUIDs           ITEMs = ItemUuids ( )        ;
  ListWidgetItems mItems                       ;
  foreach (uuid,Uuids)                         {
    QListWidgetItem * it = uuidAt ( uuid )     ;
    mItems << it                               ;
    takeItem ( row ( it ) )                    ;
    ITEMs . takeAt ( ITEMs . indexOf (uuid) )  ;
  }                                            ;
  QListWidgetItem * rit   = uuidAt ( atUuid )  ;
  int               rid   = row    ( rit    )  ;
  for (int i=0;i<mItems.count();i++,rid++)     {
    insertItem ( rid , mItems [ i ] )          ;
  }                                            ;
  //////////////////////////////////////////////
  emit assignEnabling   ( false )              ;
  plan -> processEvents (       )              ;
  //////////////////////////////////////////////
  UUIDs Puids = Uuids                          ;
  if ( isOwner ( ) )                           {
    EnterSQL ( SC , plan->sql )                ;
          GroupItems :: Join                   (
            SC                                 ,
            first                              ,
            t1                                 ,
            t2                                 ,
            relation                           ,
            atUuid                             ,
            Puids                            ) ;
    LeaveSQL ( SC , plan->sql )                ;
  }                                            ;
  //////////////////////////////////////////////
  emit assignEnabling   ( true  )              ;
  plan -> processEvents (       )              ;
  setSelections         ( Puids )              ;
  ALERT                 ( Done  )              ;
  dropAction = false                           ;
}

void N::PictureView::Import(void)
{
  QString dir = plan->Dirs[Directory::Images].absoluteFilePath("") ;
  if ( plan->Variables.contains("PictureViewDir") )                {
    dir = plan -> Variables [ "PictureViewDir" ] . toString ( )    ;
  }                                                                ;
  QStringList files = QFileDialog::getOpenFileNames                (
                        this                                       ,
                        tr("Import pictures")                      ,
                        dir                                        ,
                        N::Images::ImageFilters()                ) ;
  nDropOut ( files.count() <= 0 )                                  ;
  QFileInfo LFD ( files . last ( ) )                               ;
  plan -> Variables [ "PictureViewDir" ] = LFD . absolutePath ( )  ;
  //////////////////////////////////////////////////////////////////
  VarArgs args                                                     ;
  for (int i = 0 ; i < files . count ( ) ; i++ )                   {
    args << files [ i ]                                            ;
  }                                                                ;
  start ( 40007 , args )                                           ;
}

bool N::PictureView::Import(QStringList Files)
{
  if ( Files . count ( ) <= 0 ) return false                           ;
  //////////////////////////////////////////////////////////////////////
  qint64 index = 0                                                     ;
  qint64 total = 0                                                     ;
  int    id    = -1                                                    ;
  bool   go    = true                                                  ;
  //////////////////////////////////////////////////////////////////////
  UUIDs Uuids                                                          ;
  EnterSQL ( SC , plan->sql )                                          ;
    SUID  uuid                                                         ;
    id = plan -> Progress ( LocalMsgs [ PixMsgImport   ]               ,
                            LocalMsgs [ PixMsgProgress ]             ) ;
    plan -> Start    ( id , &index , &go                             ) ;
    plan -> setRange ( id , 0      , Files . count ( )               ) ;
    for (int i = 0 ; go && ( i < Files . count ( ) ) ; i++ , index++ ) {
      plan -> showMessage   ( Files [ i ] )                            ;
      plan -> processEvents (             )                            ;
      uuid = 0                                                         ;
      PictureManager :: Import ( SC , Files[i] , uuid )                ;
      if ( uuid > 0 )                                                  {
        if ( ! Uuids . contains ( uuid ) ) Uuids << uuid               ;
        total++                                                        ;
      }                                                                ;
    }                                                                  ;
    plan -> Finish ( id )                                              ;
  LeaveSQL ( SC , plan->sql )                                          ;
  //////////////////////////////////////////////////////////////////////
  if ( isOwner ( ) ) Join ( first , 0 , Uuids )                        ;
  //////////////////////////////////////////////////////////////////////
  QString m = QString ( LocalMsgs [ PixMsgToDB ] ) . arg ( total )     ;
  plan -> showMessage ( m )                                            ;
  plan -> Talk        ( m )                                            ;
  return true                                                          ;
}

void N::PictureView::GalleryIcon(SUID FIRST,int TYPE,SUID ICON)
{
  EnterSQL ( SC , plan->sql   )    ;
    bool correct                   ;
    correct = Delegate             (
                SC                 ,
                FIRST              ,
                ICON               ,
                TYPE               ,
                Types::Picture     ,
                Groups::Icon     ) ;
    if ( correct )                 {
      emit Update ( this , FIRST ) ;
      ALERT ( Done  )              ;
    } else                         {
      ALERT ( Error )              ;
    }                              ;
  LeaveSQL ( SC , plan->sql   )    ;
}

void N::PictureView::GalleryIcon(VarArgs & V)
{
  if (V.count()<3) return            ;
  SUID F = V [ 0 ] . toULongLong ( ) ;
  SUID T = V [ 1 ] . toInt       ( ) ;
  SUID U = V [ 2 ] . toULongLong ( ) ;
  GalleryIcon ( F , T , U )          ;
}

void N::PictureView::GalleryIcon(void)
{
  if ( ! isFirst ( ) ) return    ;
  QListWidgetItem * it           ;
  it = currentItem ( )           ;
  nDropOut ( IsNull(it)        ) ;
  SUID u = nListUuid(it)         ;
  nDropOut    ( u <= 0         ) ;
  ////////////////////////////////
  VarArgs V                      ;
  V << 30101                     ;
  V << first                     ;
  V << t1                        ;
  V << u                         ;
  addSequence  ( V )             ;
  StartCommand (   )             ;
}

void N::PictureView::DetachIcon(void)
{
  if (! isFirst ( ) ) return     ;
  EnterSQL ( SC , plan->sql    ) ;
    GroupItems :: FullDetach     (
      SC                         ,
      first                      ,
      t1                         ,
      Types  :: Picture          ,
      Groups :: Icon           ) ;
    emit Update ( this , first ) ;
  LeaveSQL ( SC , plan->sql    ) ;
  ALERT    ( Done              ) ;
}

void N::PictureView::ReportId(QListWidgetItem * it)
{
  nDropOut ( IsNull(it)     ) ;
  SUID u = nListUuid(it)      ;
  nDropOut ( u <= 0         ) ;
  EnterSQL ( SC , plan->sql ) ;
    if (PictureId(SC,it))     {
      ALERT ( Done  )         ;
    } else                    {
      ALERT ( Error )         ;
    }                         ;
  LeaveSQL ( SC , plan->sql ) ;
}

void N::PictureView::LocateOrders(void)
{
  start ( 40004 ) ;
}

void N::PictureView::LocatingOrders(ThreadData * d)
{
  emit assignEnabling  ( false            )                ;
  EnterSQL    ( SC , plan->sql   )                         ;
    QString Q                                              ;
    UUIDs   Uuids                                          ;
    SUID    uuid                                           ;
    qint64  index = 0                                      ;
    int     id    = -1                                     ;
    bool    go    = true                                   ;
    plan -> showMessage   ( LocalMsgs [ PixMsgOrganize ] ) ;
    plan -> processEvents (                              ) ;
    id = plan -> Progress ( LocalMsgs [ PixMsgOrders   ]   ,
                            LocalMsgs [ PixMsgProgress ] ) ;
    Uuids = SC . Uuids                                     (
              PlanTable(PictureOrders)                     ,
              "uuid"                                       ,
              SC.OrderByAsc("id")                        ) ;
    if ( Uuids . count ( ) > 0 )                           {
      plan -> Start    ( id , &index , &go               ) ;
      plan -> setRange ( id , 0      , Uuids . count ( ) ) ;
      foreach ( uuid , Uuids )                             {
        ++index                                            ;
        Q = SC.sql.Update                                  (
              PlanTable(PictureOrders)                     ,
              SC . sql . Where ( 1 , "uuid" )              ,
              1                                            ,
              "id"                                       ) ;
        SC . Prepare ( Q              )                    ;
        SC . Bind    ( "id"   , index )                    ;
        SC . Bind    ( "uuid" , uuid  )                    ;
        SC . Exec    (                )                    ;
      }                                                    ;
      index++                                              ;
      Q = QString ( "alter table %1 auto_increment = %2;"  )
                  . arg ( PlanTable(PictureOrders)         )
                  . arg ( index                          ) ;
      plan -> processEvents (   )                          ;
      plan -> showMessage   ( Q )                          ;
      SC . Query            ( Q )                          ;
    }                                                      ;
  LeaveSQL   ( SC , plan->sql )                            ;
  emit assignEnabling ( true  )                            ;
  ALERT      ( Done           )                            ;
}

void N::PictureView::FindBySites(void)
{
  QString     k                                          ;
  QStringList K                                          ;
  QStringList KL                                         ;
  ////////////////////////////////////////////////////////
  k = QInputDialog::getText                              (
        this                                             ,
        LocalMsgs [ PixMsgFind    ]                      ,
        LocalMsgs [ PixMsgKeyword ]                    ) ;
  if ( k . length ( ) <= 0 ) return                      ;
  KL = k . split ( ' ' )                                 ;
  for (int i = 0 ; i <  KL . count ( ) ; i++ )           {
    k = KL [ i ]                                         ;
    k = k . replace ( " " , "" )                         ;
    if ( k . length ( ) > 0 ) K << k                     ;
  }                                                      ;
  if ( K . count ( ) <= 0 ) return                       ;
  ////////////////////////////////////////////////////////
  VarArgs args                                           ;
  args <= K                                              ;
  start ( 40003 , args )                                 ;
}

void N::PictureView::FindFromSites(ThreadData * d)
{
  if ( ! IsContinue ( d )              ) return           ;
  if ( d -> Arguments . count ( ) <= 0 ) return           ;
  /////////////////////////////////////////////////////////
  QStringList K                                           ;
  UUIDs       Uuids = ItemUuids ( )                       ;
  /////////////////////////////////////////////////////////
  for (int i = 0 ; i < d -> Arguments . count ( ) ; i++ ) {
    K << d -> Arguments [ i ] . toString ( )              ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  NetworkManager NM ( plan             )                  ;
  EnterSQL          ( SC , plan->sql   )                  ;
    SUID uuid                                             ;
    SUID puid                                             ;
    plan -> showMessage   ( LocalMsgs [ PixMsgSites ]   ) ;
    plan -> processEvents (                             ) ;
    foreach (uuid,Uuids)                                  {
      UUIDs Puids                                         ;
      QListWidgetItem * it = uuidAt(uuid)                 ;
      Puids = Subordination                               (
                SC                                        ,
                uuid                                      ,
                Types::Picture                            ,
                Types::URL                                ,
                Groups::Subordination                     ,
                SC.OrderByAsc("position")               ) ;
      if (NotNull(it))                                    {
        bool selected = false                             ;
        if (Puids.count()>0)                              {
          QStringList L                                   ;
          foreach (puid,Puids)                            {
            QUrl u = NM.PageUrl(SC,puid)                  ;
            if (u.isValid()) L << u.toString()            ;
          }                                               ;
          if (L.count()>0)                                {
            for (int i=0;!selected && i<L.count();i++)    {
              QString l = L[i]                            ;
              selected = true                             ;
              for (int j=0;selected && j<K.count();j++)   {
                if (!l.contains(K[j])) selected = false   ;
              }                                           ;
            }                                             ;
          }                                               ;
        }                                                 ;
        it -> setSelected ( selected )                    ;
      }                                                   ;
    }                                                     ;
  LeaveSQL          ( SC , plan -> sql )                  ;
  ALERT             ( Done             )                  ;
}

void N::PictureView::FindByAlgebra(void)
{
}

void N::PictureView::ReportURLs(void)
{
  VarArgs V          ;
  V << 30002         ;
  V << 2             ;
  addSequence  ( V ) ;
  StartCommand (   ) ;
}

bool N::PictureView::Export(QDir & dir,UUIDs & Uuids)
{
  nKickOut ( Uuids . count ( ) <= 0 , false )                      ;
  int cnt = 0                                                      ;
  EnterSQL ( SC , plan->sql )                                      ;
    SUID    uuid                                                   ;
    QString name                                                   ;
    qint64  index = 0                                              ;
    int     id    = -1                                             ;
    bool    go    = true                                           ;
    ////////////////////////////////////////////////////////////////
    if ( ! dir . exists ( ) )                                      {
      dir . mkpath ( dir . absoluteFilePath ( "" ) )               ;
    }                                                              ;
    ////////////////////////////////////////////////////////////////
    id = plan -> Progress ( LocalMsgs [ PixMsgExporting ]          ,
                            LocalMsgs [ PixMsgProgress  ]        ) ;
    plan      -> Start    ( id , &index , &go                    ) ;
    plan      -> setRange ( id , 0      , Uuids . count ( )      ) ;
    ////////////////////////////////////////////////////////////////
    for (int i = 0 ; go && i < Uuids . count ( ) ; i++ , index++ ) {
      uuid  = Uuids [ i ]                                          ;
      name  = PictureManager :: Export ( SC , dir , uuid )         ;
      if ( name . length ( ) > 0 )                                 {
        plan -> showMessage ( name )                               ;
        cnt ++                                                     ;
      }                                                            ;
    }                                                              ;
    ////////////////////////////////////////////////////////////////
    plan      -> Finish   ( id                                   ) ;
  LeaveSQL ( SC , plan->sql )                                      ;
  QString m = QString(LocalMsgs[PixMsgExported]).arg(cnt)          ;
  alert ( "Done" , m )                                             ;
  return true                                                      ;
}

void N::PictureView::Save(void)
{
  UUIDs Uuids = Selected ( )                       ;
  nDropOut ( Uuids . count ( ) <= 0 )              ;
  //////////////////////////////////////////////////
  QString d = QFileDialog::getExistingDirectory    (
                this                               ,
                LocalMsgs [ PixMsgSaveTo ]         ,
                plan->Temporary("")                ,
                QFileDialog::ShowDirsOnly          |
                QFileDialog::DontResolveSymlinks ) ;
  nDropOut ( d.length()    <= 0 )                  ;
  //////////////////////////////////////////////////
  VarArgs args                                     ;
  args << d                                        ;
  args << Uuids . count ( )                        ;
  toVariants ( Uuids , args )                      ;
  start      ( 40002 , args )                      ;
}

void N::PictureView::SaveAll(void)
{
  if (! isOwner( ) )                               {
    ALERT ( Error )                                ;
    return                                         ;
  }                                                ;
  //////////////////////////////////////////////////
  QString d = QFileDialog::getExistingDirectory    (
                this                               ,
                LocalMsgs [ PixMsgSaveTo ]         ,
                plan->Temporary("")                ,
                QFileDialog::ShowDirsOnly          |
                QFileDialog::DontResolveSymlinks ) ;
  nDropOut ( d . length ( )  <= 0 )                ;
  //////////////////////////////////////////////////
  VarArgs args                                     ;
  args << d                                        ;
  start ( 40001 , args )                           ;
}

void N::PictureView::ExportDirectory(ThreadData * d)
{
  if ( ! IsContinue ( d )              ) return    ;
  if ( d -> Arguments . count ( ) <= 0 ) return    ;
  QString dn = d -> Arguments [ 0 ] . toString ( ) ;
  //////////////////////////////////////////////////
  UUIDs Uuids                                      ;
  EnterSQL ( SC , plan->sql )                      ;
    Uuids = Subordination                          (
              SC                                   ,
              first                                ,
              t1                                   ,
              t2                                   ,
              relation                             ,
              SC.OrderByAsc("position" )         ) ;
  LeaveSQL ( SC , plan->sql )                      ;
  if ( Uuids . count ( ) <= 0 )                    {
    ALERT ( Error )                                ;
    return                                         ;
  }                                                ;
  //////////////////////////////////////////////////
  QDir dir ( dn          )                         ;
  Export   ( dir , Uuids )                         ;
}

void N::PictureView::ExportUuids(ThreadData * d)
{
  if ( ! IsContinue ( d )             ) return      ;
  if ( d -> Arguments . count ( ) < 3 ) return      ;
  ///////////////////////////////////////////////////
  QString dn = d -> Arguments [ 0 ] . toString  ( ) ;
  int     tt = d -> Arguments [ 1 ] . toInt     ( ) ;
  UUIDs   U                                         ;
  for (int i = 0 ; i < tt ; i++ )                   {
    U << d -> Arguments [ i + 2 ] . toULongLong ( ) ;
  }                                                 ;
  ///////////////////////////////////////////////////
  QDir dir ( dn      )                              ;
  Export   ( dir , U )                              ;
}

void N::PictureView::AssignText(ThreadData * d)
{
  if ( IsNull ( d )                   ) return       ;
  if ( d -> Arguments . count ( ) < 2 ) return       ;
  SUID    u = d -> Arguments [ 0 ] . toULongLong ( ) ;
  QString t = d -> Arguments [ 1 ] . toString    ( ) ;
  if ( u <= 0                         ) return       ;
  ////////////////////////////////////////////////////
  SqlConnection SC  ( plan -> sql                  ) ;
  if                ( SC . open ( FunctionString ) ) {
    QString m                                        ;
    SC . assureName ( PlanTable(Names)               ,
                      u                              ,
                      vLanguageId                    ,
                      t                            ) ;
    m = QString     ( LocalMsgs [ PixMsgAssignText ] )
        . arg       ( u                              )
        . arg       ( t                            ) ;
    alert           ( "Done" , m                   ) ;
    SC . close      (                              ) ;
  }                                                  ;
  SC   . remove     (                              ) ;
}

void N::PictureView::AssignHtml(ThreadData * d)
{
  if ( IsNull ( d )                   ) return       ;
  if ( d -> Arguments . count ( ) < 2 ) return       ;
  SUID    u = d -> Arguments [ 0 ] . toULongLong ( ) ;
  QString t = d -> Arguments [ 1 ] . toString    ( ) ;
  if ( u <= 0                         ) return       ;
  ////////////////////////////////////////////////////
  SqlConnection SC  ( plan -> sql                  ) ;
  if                ( SC . open ( FunctionString ) ) {
    QString m                                        ;
    m = QString     ( LocalMsgs [ PixMsgAssignHtml ] )
        . arg       ( u                            ) ;
    alert           ( "Done" , m                   ) ;
    SC . close      (                              ) ;
  }                                                  ;
  SC   . remove     (                              ) ;
}

void N::PictureView::ImportFiles(ThreadData * d)
{
  if ( ! IsContinue ( d )              ) return ;
  if ( d -> Arguments . count ( ) <= 0 ) return ;
  QStringList s                                 ;
  for (int i=0;i<d->Arguments.count();i++)      {
    s << d -> Arguments [ i ] . toString ( )    ;
  }                                             ;
  Import ( s )                                  ;
}

void N::PictureView::AssignTags(ThreadData * d)
{
  if ( ! IsContinue ( d )             ) return            ;
  if ( d -> Arguments . count ( ) < 2 ) return            ;
  /////////////////////////////////////////////////////////
  UUIDs Puids                                             ;
  UUIDs Uuids                                             ;
  Puids << d -> Arguments [ 0 ] . toULongLong ( )         ;
  for (int i = 1 ; i < d -> Arguments . count ( ) ; i++ ) {
    Uuids << d -> Arguments [ i ] . toULongLong ( )       ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  SqlConnection SC ( plan -> sql )                        ;
  if ( SC . open ( FunctionString ) )                     {
    SUID u                                                ;
    foreach ( u , Uuids )                                 {
      GroupItems::Join                                    (
        SC                                                ,
        u                                                 ,
        Types::Tag                                        ,
        t2                                                ,
        relation                                          ,
        0                                                 ,
        Puids                                           ) ;
    }                                                     ;
    ALERT       ( Done )                                  ;
    SC . close  (      )                                  ;
  }                                                       ;
  SC   . remove (      )                                  ;
}

void N::PictureView::run(int Type,ThreadData * data)
{
  if ( ! IsContinue ( data ) ) return            ;
  VarArgs  V      = data -> Arguments            ;
  bool     loaded = false                        ;
  QString  TT                                    ;
  N::Group group                                 ;
  UUIDs    U                                     ;
  int      t                                     ;
  switch   ( Type )                              {
    case 10002                                   :
      TT = tr("Counting pictures...")            ;
      emit assignToolTip    ( TT               ) ;
      emit OnBusy           (                  ) ;
      startLoading          (                  ) ;
      FetchTOTALs           (                  ) ;
      stopLoading           (                  ) ;
      emit GoRelax          (                  ) ;
      V . prepend           ( QVariant(10003)  ) ;
      addSequence           ( V                ) ;
      emit TriggerCommand   (                  ) ;
    break                                        ;
    case 10004                                   :
      TT = tr("Loading pictures...")             ;
      emit assignToolTip    ( TT               ) ;
      emit OnBusy           (                  ) ;
      startLoading          (                  ) ;
      FetchUUIDs            (                  ) ;
      stopLoading           (                  ) ;
      emit GoRelax          (                  ) ;
      if   ( LimitValues[1] > 0                ) {
        if ( UuidMaps . contains ( 10001 )     ) {
          UUIDs U = UuidMaps [ 10001 ]           ;
          if (U.count()>0)                       {
            V  . clear          (              ) ;
            V << 10005                           ;
            V << U.count        (              ) ;
            toVariants          ( U , V        ) ;
            addSequence         ( V            ) ;
            emit TriggerCommand (              ) ;
            loaded = true                        ;
          }                                      ;
        }                                        ;
      }                                          ;
      ////////////////////////////////////////////
      if ( ! loaded )                            {
        TT = tr("No picture found")              ;
        emit assignToolTip  ( TT               ) ;
      }                                          ;
    break                                        ;
    case 10011                                   :
      emit OnBusy           (                  ) ;
      ArgsToGroup           ( 0 , V , group    ) ;
      ArgsToUuids           ( 7 , V , U        ) ;
      if ( group.first > 0 )                     {
        startLoading        (                  ) ;
        JoinGroupThread     ( group , U        ) ;
        stopLoading         (                  ) ;
        IsCompleted         (                  ) ;
      }                                          ;
      emit GoRelax          (                  ) ;
    break                                        ;
    case 10012                                   :
      emit OnBusy           (                  ) ;
      ArgsToGroup           ( 0 , V , group    ) ;
      ArgsToUuids           ( 7 , V , U        ) ;
      if ( group.first > 0 )                     {
        startLoading        (                  ) ;
        JoinGroupThread     ( group , U        ) ;
        stopLoading         (                  ) ;
        IsCompleted         (                  ) ;
      }                                          ;
      emit GoRelax          (                  ) ;
      ////////////////////////////////////////////
      addSequence           ( 10001            ) ;
      emit TriggerCommand   (                  ) ;
    break                                        ;
    case 10013                                   :
      emit OnBusy           (                  ) ;
      TT = V[0].toString    (                  ) ;
      V . takeAt            ( 0                ) ;
      ArgsToGroup           ( 0 , V , group    ) ;
      ArgsToUuids           ( 7 , V , U        ) ;
      if ( group.first > 0 )                     {
        startLoading        (                  ) ;
        JoinGroupThread     ( group , U        ) ;
        stopLoading         (                  ) ;
        IsCompleted         (                  ) ;
      }                                          ;
      emit GoRelax          (                  ) ;
      ////////////////////////////////////////////
      V  . clear            (                  ) ;
      V << 90011                                 ;
      V << TT                                    ;
      addSequence           ( 10001            ) ;
      addSequence           ( V                ) ;
      emit TriggerCommand   (                  ) ;
    break                                        ;
    case 30001                                   :
      emit OnBusy           (                  ) ;
      startLoading          (                  ) ;
      ThumbsThread          (                  ) ;
      stopLoading           (                  ) ;
      IsCompleted           (                  ) ;
      emit GoRelax          (                  ) ;
    break                                        ;
    case 30002                                   :
      if (V.count()>0)                           {
        t = V [ 0 ] . toInt (                  ) ;
        emit OnBusy         (                  ) ;
        startLoading        (                  ) ;
        DetailsThread       ( t                ) ;
        stopLoading         (                  ) ;
        IsCompleted         (                  ) ;
        emit GoRelax        (                  ) ;
      }                                          ;
    break                                        ;
    case 30003                                   :
      emit OnBusy           (                  ) ;
      startLoading          (                  ) ;
      ThumbsThread          ( V                ) ;
      stopLoading           (                  ) ;
      IsCompleted           (                  ) ;
      emit GoRelax          (                  ) ;
    break                                        ;
    case 30101                                   :
      startLoading          (                  ) ;
      GalleryIcon           ( V                ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 30102                                   :
      startLoading          (                  ) ;
      DetachIcon            (                  ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 40001                                   :
      startLoading          (                  ) ;
      ExportDirectory       ( data             ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 40002                                   :
      startLoading          (                  ) ;
      ExportUuids           ( data             ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 40003                                   :
      startLoading          (                  ) ;
      FindFromSites         ( data             ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 40004                                   :
      startLoading          (                  ) ;
      LocatingOrders        ( data             ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 40005                                   :
      startLoading          (                  ) ;
      DeleteByThread        ( data             ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 40006                                   :
      startLoading          (                  ) ;
      AssignText            ( data             ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 40007                                   :
      startLoading          (                  ) ;
      ImportFiles           ( data             ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 40008                                   :
      startLoading          (                  ) ;
      AssignHtml            ( data             ) ;
      stopLoading           (                  ) ;
    break                                        ;
    case 40009                                   :
      startLoading          (                  ) ;
      AssignTags            ( data             ) ;
      stopLoading           (                  ) ;
    break                                        ;
  }                                              ;
}

bool N::PictureView::RunCommand(VarArgs & arguments)
{
  if (arguments.count()<1) return false  ;
  VarArgs V = arguments                  ;
  int     c = V [ 0 ] . toInt ( )        ;
  UUIDs   U                              ;
  SUID    u                              ;
  switch ( c )                           {
    case 10001                           :
      Reload     (       )               ;
    break                                ;
    case 10003                           :
      start      ( 10004 )               ;
    break                                ;
    case 10004                           :
      start      ( 10004 )               ;
    break                                ;
    case 10005                           :
      ArgsToUuids ( 1 , V , U )          ;
      plan -> processEvents ( )          ;
      if (U.count()>0)                   {
        View     ( U                   ) ;
        plan -> processEvents (        ) ;
        setFocus ( Qt::TabFocusReason  ) ;
      } else                             {
        Alert ( Error )                  ;
      }                                  ;
    break                                ;
    case 12001                           :
      ArgsToUuids ( 1 , V , U )          ;
      plan -> processEvents ( )          ;
      setSelections ( U )                ;
    break                                ;
    case 30001                           :
      GoThumbsThreads ( 30003 )          ;
    break                                ;
    case 31001                           :
      if (V.count()<2) return true       ;
      u = V [ 1 ] . toULongLong ( )      ;
      if ( u <= 0 ) return true          ;
      emit Update ( this , u )           ;
    break                                ;
    case 10002                           :
    case 10011                           :
    case 10012                           :
    case 10013                           :
    case 30002                           :
    case 30003                           :
    case 30101                           :
    case 30102                           :
      V . takeAt ( 0     )               ;
      start      ( c , V )               ;
    break                                ;
    case 90011                           :
      V . takeAt ( 0     )               ;
      plan->showMessage(V[0].toString()) ;
    break                                ;
    default                              :
    return false                         ;
  }                                      ;
  return true                            ;
}

void N::PictureView::addPictureMenu (
       MenuManager     & mm         ,
       QListWidgetItem * it         ,
       SUID              uuid       ,
       int             & StartId    ,
       int             & PageSize   )
{
  if (NotNull(it))                                    {
    mm . add                                          (
      1                                               ,
      QString::number(uuid)                           ,
      true                                            ,
      false                                         ) ;
    mm . addSeparator (                             ) ;
  }                                                   ;
  PageMenu                                            (
    mm                                                ,
    tr("Total %1 pictures")                           ,
    StartId                                           ,
    PageSize                                        ) ;
  if (NotNull(it))                                    {
    mm   . add ( 70101 , tr("Open"                ) ) ;
  }                                                   ;
  mm     . add ( 70201 , LocalIcons [ "Update" ]      ,
                         tr("Reflush"             ) ) ;
  if ( isOwner ( ) )                                  {
    if (NotNull(it))                                  {
      mm . add ( 70111 , LocalIcons [ "Tag" ]         ,
                         tr("Assign as group icon") ) ;
    }                                                 ;
    mm . add   ( 70112 , tr("Remove group icon"   ) ) ;
  }                                                   ;
  mm   . add   ( 70104 , LocalIcons [ "Import" ]      ,
                         tr("Import pictures"     ) ) ;
}

bool N::PictureView::RunPictureMenu (
       MenuManager     & mm         ,
       QAction         * aa         ,
       QListWidgetItem * it         )
{
  switch (mm[aa])                {
    case 70101                   :
      if ( NotNull(it) )         {
        doubleClicked ( it )     ;
      }                          ;
    return true                  ;
    case 70104                   :
      Import          (        ) ;
    return true                  ;
    case 70111                   :
      GalleryIcon     (        ) ;
    return true                  ;
    case 70112                   :
      if (isFirst())             {
        addSequence   ( 30102  ) ;
        StartCommand  (        ) ;
      }                          ;
    return true                  ;
    case 70201                   :
      Reload          (        ) ;
    return true                  ;
  }                              ;
  return false                   ;
}

void N::PictureView::addEditingMenu (
       MenuManager     & mm         ,
       QMenu           * me         ,
       QListWidgetItem * it         ,
       UUIDs           & Uuids      )
{
  if ( isOwner ( ) )                                                                 {
    if (NotNull(it))                                                                 {
      mm . add ( me , 70102 , tr("Remove"           )                              ) ;
    }                                                                                ;
    if (Uuids.count()>0)                                                             {
      mm . add ( me , 70103 , tr("Remove selections")                              ) ;
    }                                                                                ;
    mm   . addSeparator ( me )                                                       ;
  }                                                                                  ;
  if (NotNull(it))                                                                   {
    mm   . add ( me , 70421 , tr("Documents")                                      ) ;
    mm   . add ( me , 70422 , tr("Download from related webpage")                  ) ;
  }                                                                                  ;
  if (Uuids.count()>0)                                                               {
    mm   . addSeparator ( me )                                                       ;
    if ( isOwner ( ) )                                                               {
      mm . add ( me , 70401 , tr("Move selected pictures to group end")            ) ;
    }                                                                                ;
    mm   . add ( me , 70402 , tr("Move selected pictures to depot end")            ) ;
    mm   . addSeparator ( me )                                                       ;
  }                                                                                  ;
  if ( isOwner ( ) )                                                                 {
    mm   . add ( me , 70403 , tr("Move all pictures in this gallery to depot end") ) ;
    mm   . addSeparator ( me )                                                       ;
    mm   . add ( me , 70410 , tr("Relocate picture orders of this group")          ) ;
  }                                                                                  ;
  mm     . add ( me , 70411 , tr("Relocate picture depot ID orders")               ) ;
}

bool N::PictureView::RunEditingMenu (
       MenuManager     & mm         ,
       QAction         * aa         ,
       QListWidgetItem * it         ,
       UUIDs           & Uuids      )
{
  SUID u = 0                                    ;
  if (NotNull(it))                              {
    u = nListUuid    ( it                     ) ;
  }                                             ;
  switch (mm[aa])                               {
    case 70102                                  :
      DeleteThis     (                        ) ;
    return true                                 ;
    case 70103                                  :
      Delete         (                        ) ;
    return true                                 ;
    case 70311                                  :
      FindBySites    (                        ) ;
    return true                                 ;
    case 70312                                  :
      FindByAlgebra  (                        ) ;
    return true                                 ;
    case 70401                                  :
      if (Uuids.count()>0)                      {
        MoveToEnd      ( first , Uuids        ) ;
        Reload         (                      ) ;
        setSelections  ( Uuids                ) ;
        ALERT          ( Done                 ) ;
      }                                         ;
    return true                                 ;
    case 70402                                  :
      if (Uuids.count()>0)                      {
        MoveToEnd      ( Uuids                ) ;
        Reload         (                      ) ;
        setSelections  ( Uuids                ) ;
        ALERT          ( Done                 ) ;
      }                                         ;
    return true                                 ;
    case 70403                                  :
      MoveAllToEnd   (                        ) ;
    return true                                 ;
    case 70410                                  :
      Relocate       ( first , t1 , relation  ) ;
    return true                                 ;
    case 70411                                  :
      LocateOrders   (                        ) ;
    return true                                 ;
    case 70421                                  :
      emit Documents ( windowTitle() , u , t1 ) ;
    return true                                 ;
    case 70422                                  :
      if (Uuids.count()>0)                      {
        emit DownloadUUIDs ( Uuids            ) ;
      }                                         ;
    return true                                 ;
  }                                             ;
  return false                                  ;
}

void N::PictureView::addCatalogMenu (
       MenuManager     & mm         ,
       QListWidgetItem * it         ,
       UUIDs           & Uuids      )
{ Q_UNUSED          ( it                             ) ;
  Q_UNUSED          ( Uuids                          ) ;
  if ( ! isOwner ( ) ) return                          ;
  QMenu * mc                                           ;
  mc = mm . addMenu (              tr("Categorize" ) ) ;
  mm . add          ( mc , 70501 , tr("Constraints") ) ;
  mm . add          ( mc , 70502 , tr("Rule tables") ) ;
  mm . add          ( mc , 70503 , tr("Subgroups"  ) ) ;
}

bool N::PictureView::RunCatalogMenu (
       MenuManager     & mm         ,
       QAction         * aa         ,
       QListWidgetItem * it         ,
       UUIDs           & Uuids      )
{
  Q_UNUSED               ( Uuids                  ) ;
  if ( ! isOwner ( ) ) return false                 ;
  if ( IsNull(it)    ) return false                 ;
  SUID u = nListUuid ( it )                         ;
  if ( u <= 0 ) return false                        ;
  switch (mm[aa])                                   {
    case 70501                                      :
      emit Constraints   ( windowTitle() , u , t1 ) ;
    return true                                     ;
    case 70502                                      :
      emit RuleTables    ( windowTitle() , u , t1 ) ;
    return true                                     ;
    case 70503                                      :
      emit ListSubgroups ( windowTitle() , u , t1 ) ;
    return true                                     ;
  }                                                 ;
  return false                                      ;
}

void N::PictureView::addDetailsMenu (
       MenuManager     & mm         ,
       QMenu           * mi         ,
       QListWidgetItem * it         ,
       UUIDs           & Uuids      )
{ Q_UNUSED            ( Uuids                                  ) ;
  if (NotNull(it))                                               {
    mm . add          ( mi , 70701 , tr("Picture order Id"   ) ) ;
  }                                                              ;
  mm   . add          ( mi , 70702 , tr("Related web pages"  ) ) ;
  mm   . add          ( mi , 70703 , tr("Related people"     ) ) ;
  mm   . addSeparator ( mi                                     ) ;
  mm   . add          ( mi , 70711 , tr("All related items"  ) ) ;
  mm   . addSeparator ( mi                                     ) ;
  mm   . add          ( mi , 70704 , tr("Report picture URLs") ) ;
}

bool N::PictureView::RunDetailsMenu (
       MenuManager     & mm         ,
       QAction         * aa         ,
       QListWidgetItem * it         ,
       UUIDs           & Uuids      )
{ Q_UNUSED               ( Uuids                  ) ;
  SUID u = 0                                        ;
  if (NotNull(it))                                  {
    u = nListUuid        ( it                     ) ;
  }                                                 ;
  switch (mm[aa])                                   {
    case 70701                                      :
      ReportId           ( it                     ) ;
    return true                                     ;
    case 70702                                      :
      emit Webpages      ( windowTitle() , u      ) ;
    return true                                     ;
    case 70703                                      :
      emit RelatedPeople ( windowTitle() , u , t1 ) ;
    return true                                     ;
    case 70704                                      :
      ReportURLs         (                        ) ;
    return true                                     ;
    case 70711                                      :
      emit AllRelated    ( windowTitle() , u , t1 ) ;
    return true                                     ;
    default                                         :
    break                                           ;
  }                                                 ;
  return false                                      ;
}

void N::PictureView::addProcessMenu (
       MenuManager     & mm         ,
       QListWidgetItem * it         ,
       UUIDs           & Uuids      )
{ Q_UNUSED          ( it                                             ) ;
  if ( Uuids.count() <= 0 ) return                                     ;
  QMenu * mp                                                           ;
  mp = mm . addMenu ( tr("Image processing")                         ) ;
  mm . add          ( mp , 70801 , tr("Filters"                    ) ) ;
  mm . add          ( mp , 70811 , tr("Facial recognition"         ) ) ;
  mm . add          ( mp , 70812 , tr("Stitching selected pictures") ) ;
}

bool N::PictureView::RunProcessMenu (
       MenuManager     & mm         ,
       QAction         * aa         ,
       QListWidgetItem * it         ,
       UUIDs           & Uuids      )
{ Q_UNUSED         ( it                              ) ;
  if ( Uuids . count ( ) <= 0 ) return false           ;
  switch (mm[aa])                                      {
    case 70801                                         :
      emit Filters ( windowTitle ( ) , first , Uuids ) ;
    return true                                        ;
    case 70811                                         :
      emit Facial  ( windowTitle ( ) , first , Uuids ) ;
    return true                                        ;
    case 70812                                         :
      emit Stitch  ( windowTitle ( ) , first , Uuids ) ;
    return true                                        ;
  }                                                    ;
  return false                                         ;
}

bool N::PictureView::Menu(QPoint pos)
{
  nKickOut ( ! hasMenu , true )                              ;
  ////////////////////////////////////////////////////////////
  MenuManager       mm(this)                                 ;
  QMenu           * ma                                       ;
  QMenu           * me                                       ;
  QMenu           * mi                                       ;
  QMenu           * ms                                       ;
  QAction         * aa                                       ;
  QListWidgetItem * it                                       ;
  SUID              uuid = 0                                 ;
  UUIDs             Uuids                                    ;
  int               StartId  = 0                             ;
  int               PageSize = 0                             ;
  ////////////////////////////////////////////////////////////
  it = itemAt(pos)                                           ;
  if (NotNull(it) && !it->isSelected()) it = NULL            ;
  if (NotNull(it)) uuid = nListUuid(it)                      ;
  Uuids = ListWidget :: Selected ( )                         ;
  ////////////////////////////////////////////////////////////
  addPictureMenu ( mm , it , uuid , StartId , PageSize   )   ;
  mm . addSeparator ( )                                      ;
  me = mm . addMenu ( tr("Edit"       )    )                 ;
  mi = mm . addMenu ( tr("Information")    )                 ;
  addCatalogMenu    ( mm ,      it , Uuids )                 ;
  addEditingMenu    ( mm , me , it , Uuids )                 ;
  addProcessMenu    ( mm ,      it , Uuids )                 ;
  mm . addSeparator (                      )                 ;
  ma = mm . addMenu ( tr("Adjustments")    )                 ;
  ms = mm . addMenu ( tr("Selections" )    )                 ;
  ////////////////////////////////////////////////////////////
  mm . add(ms,311,tr("Select pictures by related webpages")) ;
  mm . add(ms,312,tr("Select pictures by sets algebra"    )) ;
  mm . addSeparator ( ms                   )                 ;
  SelectionsMenu    ( mm , ms              )                 ;
  addDetailsMenu    ( mm , mi , it , Uuids )                 ;
  AdjustMenu        ( mm , ma              )                 ;
  DockingMenu       ( mm                   )                 ;
  ////////////////////////////////////////////////////////////
  mm . setFont   ( plan )                                    ;
  aa = mm . exec (      )                                    ;
  if (IsNull(aa))                                            {
    PageChanged(StartId,PageSize)                            ;
    return true                                              ;
  }                                                          ;
  ////////////////////////////////////////////////////////////
  if ( NotNull(it) ) setCurrentItem ( it )                   ;
  if ( RunDocking     ( mm,aa          ) ) return true       ;
  if ( RunAdjustment  ( mm,aa          ) ) return true       ;
  if ( RunSelections  ( mm,aa          ) ) return true       ;
  if ( RunPictureMenu ( mm,aa,it       ) ) return true       ;
  if ( RunEditingMenu ( mm,aa,it,Uuids ) ) return true       ;
  if ( RunCatalogMenu ( mm,aa,it,Uuids ) ) return true       ;
  if ( RunDetailsMenu ( mm,aa,it,Uuids ) ) return true       ;
  if ( RunProcessMenu ( mm,aa,it,Uuids ) ) return true       ;
  ////////////////////////////////////////////////////////////
  RunPageMenu         ( mm , aa , StartId , PageSize )       ;
  PageChanged         ( StartId , PageSize           )       ;
  return true                                                ;
}
