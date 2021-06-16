#include <qtphotography.h>

N::PaletteLists:: PaletteLists   ( QWidget * parent , Plan * p )
                : ListDock       (           parent ,        p )
                , Ownership      ( 0                           )
                , PictureManager (                           p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PaletteLists::~PaletteLists (void)
{
}

QSize N::PaletteLists::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( 640 , 480 ) ) ;
}

void N::PaletteLists::Configure (void)
{
  setAccessibleName            ( "N::PaletteLists"                ) ;
  setObjectName                ( "N::PaletteLists"                ) ;
  setWindowTitle               ( tr("Palettes")                   ) ;
  setWindowIcon                ( plan -> Icon(Types::Palette,1,0) ) ;
  ///////////////////////////////////////////////////////////////////
  setViewMode                  ( IconMode                         ) ;
  setIconSize                  ( QSize(128,128)                   ) ;
  setGridSize                  ( QSize(136,168)                   ) ;
  setMovement                  ( Snap                             ) ;
  setResizeMode                ( Adjust                           ) ;
  setSelectionMode             ( ExtendedSelection                ) ;
  setWordWrap                  ( true                             ) ;
  setMouseTracking             ( true                             ) ;
  setWrapping                  ( true                             ) ;
  setTextElideMode             ( Qt::ElideNone                    ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded            ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded            ) ;
  setMinimumSize               ( QSize(144,192)                   ) ;
  setDragDropMode              ( DragDrop                         ) ;
  plan -> setFont              ( this                             ) ;
  StackSize          = 0                                            ;
  Reservation        = false                                        ;
  ///////////////////////////////////////////////////////////////////
  LimitValues [  0 ] =  0                                           ;
  LimitValues [  1 ] = 40                                           ;
  LimitValues [  2 ] =  0                                           ;
  LimitValues [  9 ] = 40                                           ;
  LimitValues [ 11 ] = (int)Qt::AscendingOrder                      ;
  LimitValues [ 75 ] = 0                                            ;
  LimitValues [ 76 ] = 100                                          ;
  LimitValues [ 77 ] = 500                                          ;
}

bool N::PaletteLists::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , false           ) ;
  DisableAllActions (                                ) ;
  AssignAction      ( Label        , windowTitle ( ) ) ;
  LinkAction        ( Refresh      , startup     ( ) ) ;
  LinkAction        ( Rename       , Rename      ( ) ) ;
  LinkAction        ( GoHome       , PageHome    ( ) ) ;
  LinkAction        ( GoEnd        , PageEnd     ( ) ) ;
  LinkAction        ( PreviousPage , PageUp      ( ) ) ;
  LinkAction        ( NextPage     , PageDown    ( ) ) ;
  return true                                          ;
}

int N::PaletteLists::Count(SqlConnection & SC)
{
  QString Q                                                ;
  if ( isStandby ( ) )                                     {
    LimitValues [ 2 ] = SC . count ( PlanTable(Palettes) ) ;
  } else                                                   {
    GroupItems GI ( plan )                                 ;
    GI . AutoMap      = true                               ;
    GI . GroupTable   = GI . LookTable                     (
                          ObjectType ( )                   ,
                          Types::Palette                   ,
                          Connexion  ( )                 ) ;
    LimitValues [ 2 ] = GI . Count                         (
                          SC                               ,
                          ObjectUuid ( )                   ,
                          ObjectType ( )                   ,
                          Types::Palette                   ,
                          Connexion  ( )                 ) ;
  }                                                        ;
  return LimitValues [ 2 ]                                 ;
}

UUIDs N::PaletteLists::GetUuids(SqlConnection & SC)
{
  QString Q                                               ;
  UUIDs   Uuids                                           ;
  int     startId = LimitValues [ 0 ]                     ;
  int     page    = LimitValues [ 1 ]                     ;
  int     total   = LimitValues [ 2 ]                     ;
  nKickOut ( total <= 0 , Uuids )                         ;
  /////////////////////////////////////////////////////////
  if ( isStandby ( ) )                                    {
    Q = SC . sql . SelectFrom                             (
          "uuid"                                          ,
          PlanTable(Palettes)                             ,
          QString ( "%1 %2"                               )
          . arg ( SC . OrderByAsc  ( "id"           )     )
          . arg ( SC . sql . Limit ( startId , page ) ) ) ;
    SqlLoopNow ( SC , Q )                                 ;
      Uuids << SC . Uuid ( 0 )                            ;
    SqlLoopErr ( SC , Q )                                 ;
    SqlLoopEnd ( SC , Q )                                 ;
  } else                                                  {
    GroupItems GI ( plan )                                ;
    GI . AutoMap      = true                              ;
    GI . GroupTable   = GI . LookTable                    (
                          ObjectType ( )                  ,
                          Types::Palette                  ,
                          Connexion  ( )                ) ;
    Uuids = GI . Subordination                            (
              SC                                          ,
              ObjectUuid ( )                              ,
              ObjectType ( )                              ,
              Types::Palette                              ,
              Connexion  ( )                              ,
              SC . OrderByAsc ( "position" )            ) ;
    if ( page > 0 )                                       {
      UUIDs Puids                                         ;
      int   s = startId                                   ;
      int   t = Uuids . count ( )                         ;
      for (int i = 0 ; i < page ; i++ , s++ )             {
        if ( s < t ) Puids << Uuids [ s ]                 ;
      }                                                   ;
      Uuids = Puids                                       ;
    }                                                     ;
  }                                                       ;
  return Uuids                                            ;
}

void N::PaletteLists::Loading(ThreadData * d)
{
  WaitClear           (                                                    ) ;
  emit assignEnabling ( false                                              ) ;
  EnterSQL            ( SC , plan -> sql                                   ) ;
    UUIDs Uuids                                                              ;
    SUID  u                                                                  ;
    Count             ( SC                                                 ) ;
    Uuids = GetUuids  ( SC                                                 ) ;
    for (int i = 0 ; IsContinue ( d ) && ( i < Uuids . count ( ) ) ; i++   ) {
      NewListWidgetItem ( it )                                               ;
      Palette palette                                                        ;
      QIcon   icon                                                           ;
      QString name                                                           ;
      u = Uuids [ i ]                                                        ;
      if ( GetPalette ( SC , u , palette ) )                                 {
        name = GetName ( SC , u )                                            ;
        Paint                                                                (
          icon                                                               ,
          QSize ( 128 , 128 )                                                ,
          palette                                                            ,
          QSize (   8 ,  8 )                                               ) ;
        it -> setData ( Qt::UserRole , u                                   ) ;
        it -> setIcon ( icon                                               ) ;
        if ( name . length ( ) > 0 ) it -> setText ( name )                  ;
        QListWidget::addItem ( it )                                          ;
      }                                                                      ;
    }                                                                        ;
  LeaveSQL    ( SC , plan->sql )                                             ;
  ////////////////////////////////////////////////////////////////////////////
  emit assignEnabling ( true                                               ) ;
  QString m                                                                  ;
  m =              tr ( "Total %1 palettes, displayed %2 palettes"           )
                . arg ( LimitValues [ 2 ]                                    )
                . arg ( count ( )                                          ) ;
  emit assignToolTip  ( m                                                  ) ;
  plan -> Talk        ( m                                                  ) ;
  Alert               ( Done                                               ) ;
}

void N::PaletteLists::run(int T,ThreadData * d)
{
  nDropOut         ( ! IsContinue ( d ) ) ;
  switch           ( T                  ) {
    case 10001                            :
      startLoading (                    ) ;
      Loading      ( d                  ) ;
      stopLoading  (                    ) ;
    break                                 ;
  }                                       ;
}

bool N::PaletteLists::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::PaletteLists::Rename(void)
{
  QListWidgetItem * it = currentItem ( ) ;
  if ( IsNull ( it ) ) return            ;
  Rename ( it )                          ;
}

void N::PaletteLists::Rename(QListWidgetItem * item)
{
  nDropOut ( IsNull(item) )                          ;
  setCurrentItem ( item )                            ;
  ItemEditing = item                                 ;
  QRect       R = visualItemRect(item)               ;
  QLineEdit * l = new QLineEdit(this)                ;
  QFont       f = font()                             ;
  QRect       L                                      ;
  L . setLeft   ( R . left      ()     )             ;
  L . setTop    ( R . bottom    ()     )             ;
  L . setWidth  ( R . width     ()     )             ;
  L . setHeight ( f . pixelSize () + 2 )             ;
  setItemWidget ( ItemEditing , l )                  ;
  l -> setGeometry ( L )                             ;
  l -> setFont     ( f )                             ;
  ItemWidget = l                                     ;
  l -> setText  ( item -> text ( )   )               ;
  nConnect ( l    , SIGNAL ( editingFinished ( ) )   ,
             this , SLOT   ( renameFinished  ( ) ) ) ;
  l -> setFocus ( Qt::TabFocusReason )               ;
}

void N::PaletteLists::renameFinished(void)
{
  QLineEdit * l = Casting(QLineEdit,ItemWidget) ;
  nDropOut ( IsNull(l) )                        ;
  ///////////////////////////////////////////////
  QString name = l->text()                      ;
  SUID    u    = nListUuid(ItemEditing)         ;
  removeItemWidget(ItemEditing)                 ;
  if (name.length()<=0)                         {
    ItemEditing = NULL                          ;
    ItemWidget  = NULL                          ;
    return                                      ;
  }                                             ;
  ItemEditing->setText(name)                    ;
  nDropOut ( u <= 0 )                           ;
  ///////////////////////////////////////////////
  Bustle           (           )                ;
  SqlConnection SC ( plan->sql )                ;
  if ( SC . open ( FunctionString ) )           {
    SC . assureName                             (
      PlanTable(Names)                          ,
      u                                         ,
      vLanguageId                               ,
      name                                    ) ;
    SC . close( )                               ;
  }                                             ;
  SC . remove ( )                               ;
  Vacancy     ( )                               ;
  ///////////////////////////////////////////////
  ItemEditing = NULL                            ;
  ItemWidget  = NULL                            ;
  Alert ( Done )                                ;
}

bool N::PaletteLists::Menu(QPoint pos)
{
  QListWidgetItem * item = itemAt(pos)                         ;
  MenuManager       mm (this)                                  ;
  QMenu           * mv                                         ;
  QMenu           * me                                         ;
  QAction         * aa                                         ;
  int               StartId  = 0                               ;
  int               PageSize = 0                               ;
  //////////////////////////////////////////////////////////////
  if ( LimitValues [ 2 ] > 0 )                                 {
    PageMenu                                                   (
      mm                                                       ,
      tr("Total %1 palettes")                                  ,
      StartId                                                  ,
      PageSize                                               ) ;
    mm . addSeparator ( )                                      ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  mm . add ( 101 , tr("Refresh")          )                    ;
  if ( LimitValues [ 2 ] > 0 )                                 {
    me = mm . addMenu ( tr("Edit") )                           ;
    if ( NotNull(item) )                                       {
      mm . add ( me , 301 , tr("Values") )                     ;
      mm . add (      302 , tr("Rename") )                     ;
    }                                                          ;
    AdjustMenu   ( mm , me )                                   ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  DockingMenu    ( mm      )                                   ;
  //////////////////////////////////////////////////////////////
  mm . setFont   ( plan    )                                   ;
  aa = mm . exec (         )                                   ;
  //////////////////////////////////////////////////////////////
  if ( IsNull ( aa ) )                                         {
    PageChanged ( StartId , PageSize )                         ;
    return true                                                ;
  }                                                            ;
  if ( RunDocking    ( mm , aa ) ) return true                 ;
  if ( RunAdjustment ( mm , aa ) ) return true                 ;
  //////////////////////////////////////////////////////////////
  switch             ( mm [ aa ]                             ) {
    case 101                                                   :
      startup        (                                       ) ;
    return true                                                ;
    case 301                                                   :
      emit ValueList ( item -> text ( ) , nListUuid(item)    ) ;
    return true                                                ;
    case 302                                                   :
      Rename         ( item                                  ) ;
    return true                                                ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  RunPageMenu        ( mm , aa , StartId , PageSize          ) ;
  PageChanged        (           StartId , PageSize          ) ;
  //////////////////////////////////////////////////////////////
  return true                                                  ;
}
