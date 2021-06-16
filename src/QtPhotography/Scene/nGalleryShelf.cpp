#include <qtphotography.h>

#ifdef QT_OPENGL_LIB

N::GalleryShelf:: GalleryShelf ( QObject * parent , Plan * p )
                : ViewShelf    (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::GalleryShelf:: GalleryShelf ( QObject * parent , Plan * p , SceneDestructor * des )
                : ViewShelf    (           parent ,        p ,                   des )
{
  Configure ( ) ;
}

N::GalleryShelf::~GalleryShelf (void)
{
}

void N::GalleryShelf::Configure(void)
{
  QObject::setProperty ( "Type"         , "GalleryShelf"           ) ;
  setVariable          ( "Type"         , "GalleryShelf"           ) ;
  setVariable          ( "Menu"         , true                     ) ;
  ////////////////////////////////////////////////////////////////////
  setVariable          ( "Access"       , "Divisions"              ) ;
  setVariable          ( "T1"           , Types::Gallery           ) ;
  setVariable          ( "T2"           , Types::Gallery           ) ;
  setVariable          ( "T3"           , Types::Gallery           ) ;
  setVariable          ( "T4"           , Types::Gallery           ) ;
  setVariable          ( "T5"           , Types::Picture           ) ;
  setVariable          ( "Sorting"      , Qt::AscendingOrder       ) ;
  setVariable          ( "AutoRefresh"  , false                    ) ;
  setVariable          ( "ReplaceIcon"  , ":/images/gallery.png"   ) ;
  setVariable          ( "ArrangeBy"    , "Icon"                   ) ;
  setVariable          ( "ScrollBy"     , "Current"                ) ;
  ////////////////////////////////////////////////////////////////////
  setVariable          ( "Editable"     , true                     ) ;
//  setVariable          ( "FollowWindow" , true                     ) ;
  ////////////////////////////////////////////////////////////////////
  setVariable          ( "NewGroup"     , tr("New gallery")        ) ;
  setVariable          ( "ItemCount"    , tr("%1 pictures")        ) ;
  ////////////////////////////////////////////////////////////////////
  setDropFlag          ( DropText       , false                    ) ;
  setDropFlag          ( DropUrls       , false                    ) ;
  setDropFlag          ( DropImage      , false                    ) ;
  setDropFlag          ( DropHtml       , false                    ) ;
  setDropFlag          ( DropColor      , false                    ) ;
  setDropFlag          ( DropTag        , false                    ) ;
  setDropFlag          ( DropPicture    , true                     ) ;
  setDropFlag          ( DropPeople     , false                    ) ;
  setDropFlag          ( DropVideo      , false                    ) ;
  setDropFlag          ( DropAlbum      , false                    ) ;
  setDropFlag          ( DropGender     , false                    ) ;
  setDropFlag          ( DropDivision   , true                     ) ;
}

bool N::GalleryShelf::FocusIn(void)
{
  if ( IsNull ( plan ) ) return false    ;
  LinkAction ( Insert , NewGallery ( ) ) ;
  return true                            ;
}

bool N::GalleryShelf::hasItem (
       ControlPoint & From    ,
       ControlPoint & To      ,
       ControlPoint & At      )
{
  UUIDs U = SelectedItems ( )  ;
  return ( U . count ( ) > 0 ) ;
}

QMimeData * N::GalleryShelf::dragMime (
              ControlPoint & From     ,
              ControlPoint & To       ,
              ControlPoint & At       )
{
  UUIDs        U    = SelectedItems ( )           ;
  QMimeData  * mime = new QMimeData ( )           ;
  if ( 1 == U . count ( ) )                       {
    SUID       t    = U [ 0 ]                     ;
    QByteArray data = CreateByteArray ( t )       ;
    mime -> setData ( "division/uuid"  , data )   ;
  } else                                          {
    QByteArray data = CreateByteArray ( U )       ;
    mime -> setData ( "division/uuids" , data )   ;
  }                                               ;
  QString tooltip                                 ;
  tooltip = tr ( "%1 galleries selected"          )
            .arg ( U . count ( )                ) ;
  QToolTip :: showText ( QCursor::pos(),tooltip ) ;
  return mime                                     ;
}

bool N::GalleryShelf::dropPictures (
       QWidget     * widget        ,
       ControlPoint  pos           ,
       const UUIDs & Uuids         )
{
  UUIDs   U = Uuids                                  ;
  SUID    p = Value ( "DropUUID" ) . toULongLong ( ) ;
  VarArgs args                                       ;
  if ( U . count ( ) <= 0 ) return true              ;
  args << p                                          ;
  toVariants ( U , args )                            ;
  start ( CiosMenuId ( 85 , 12 , 25 ) , args )       ;
  return true                                        ;
}

bool N::GalleryShelf::dropDivisions (
       QWidget     * source         ,
       ControlPoint  psf            ,
       const UUIDs & Uuids          )
{
  UUIDs   U = Uuids                                  ;
  SUID    p = Value ( "DropUUID" ) . toULongLong ( ) ;
  VarArgs args                                       ;
  if ( U . count ( ) <= 0 ) return true              ;
  args << p                                          ;
  toVariants ( U , args )                            ;
  start ( CiosMenuId ( 85 , 12 , 32 ) , args )       ;
  return true                                        ;
}

// PickView::ItemRunner     => 10001 , 10002 , 10003 , 10004 , 10011, 73002, 98011, 98012, 851226, 851227, 851228, 851229, 851230, 851231, 851232
// GalleryShelf::ItemRunner => 73007 , 73008
bool N::GalleryShelf::ItemRunner(int type,ThreadData * data)
{
  if ( PickView::ItemRunner ( type , data ) ) return true ;
  switch ( type )                                         {
    case 73007                                            :
      GalleryConstraints ( data -> Arguments )            ;
    return true                                           ;
    case 73008                                            :
      GalleryRules       ( data -> Arguments )            ;
    return true                                           ;
  }                                                       ;
  return false                                            ;
}

void N::GalleryShelf::GalleryConstraints(VarArgs & args)
{
  if ( args . count ( ) <  2 ) return         ;
  /////////////////////////////////////////////
  SUID nuid = args [ 0 ] . toULongLong ( )    ;
  SUID uuid = args [ 1 ] . toULongLong ( )    ;
  /////////////////////////////////////////////
  if ( nuid             <= 0 ) return         ;
  if ( uuid             <= 0 ) return         ;
  /////////////////////////////////////////////
#pragma message("Gallery constraints are required to be completed")
}

void N::GalleryShelf::GalleryRules(VarArgs & args)
{
  if ( args . count ( ) <  2 ) return         ;
  /////////////////////////////////////////////
  SUID nuid = args [ 0 ] . toULongLong ( )    ;
  SUID uuid = args [ 1 ] . toULongLong ( )    ;
  /////////////////////////////////////////////
  if ( nuid             <= 0 ) return         ;
  if ( uuid             <= 0 ) return         ;
  /////////////////////////////////////////////
#pragma message("Gallery rule tables are required to be completed")
}

void N::GalleryShelf::AllPictures(void)
{
  emit ShowAll ( ) ;
}

void N::GalleryShelf::NewGallery(void)
{
  start ( CiosMenuId  ( 85 , 12 , 28 ) ) ;
}

int N::GalleryShelf::Indexing(RangeBar * rbu,int direction,QVariants & v)
{
//  qint64 s = v [ 0 ] . toLongLong ( )     ; // start
//  qint64 e = v [ 1 ] . toLongLong ( )     ; // end
//  qint64 p = v [ 2 ] . toLongLong ( )     ; // page
  qint64 i = v [ 3 ] . toLongLong ( )     ; // index
  UUIDs  U = CurrentItems                 ;
  setVariable  ( "CurrentPageIndex" , i ) ;
  PickItem::ArrangeItems ( i , U )        ;
  return 0                                ;
}

bool N::GalleryShelf::Relocation(QRectF viewport)
{
  return CentralRelocation ( viewport ) ;
}

void N::GalleryShelf::SelectNothing(void)
{
  UUIDs U = CurrentItems                          ;
  SUID  u                                         ;
  foreach ( u , U )                               {
    SelectItem * item = ChildOf<SelectItem> ( u ) ;
    if ( NotNull ( item ) )                       {
      item -> setChecked ( false )                ;
    }                                             ;
  }                                               ;
  setVariable ( "CurrentNode" , 0 )               ;
}

void N::GalleryShelf::SelectAll(void)
{
  UUIDs U = CurrentItems                          ;
  SUID  u                                         ;
  foreach ( u , U )                               {
    SelectItem * item = ChildOf<SelectItem> ( u ) ;
    if ( NotNull ( item ) )                       {
      item -> setChecked ( true )                 ;
    }                                             ;
  }                                               ;
}

void N::GalleryShelf::atDoubleClicked(SelectItem * item)
{
  PushCommand ( CiosMenuId ( 89 , 44 , 61 ) , item ) ;
}

bool N::GalleryShelf::Menu   (
       QWidget      * widget ,
       QPointF        pos    ,
       ControlPoint & From   ,
       ControlPoint & To     )
{
  AllowPickItemMenu                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  MenuManager  mm ( widget )                                                 ;
  QAction    * aa                                                            ;
  QMenu      * ma                                                            ;
  QMenu      * ms                                                            ;
  QMenu      * md                                                            ;
  QMenu      * me                                                            ;
  QMenu      * mc                                                            ;
  SelectItem * item = NearestItem ( From , To )                              ;
  QLineEdit  * line = NULL                                                   ;
  QLineEdit  * utem = NULL                                                   ;
  VarArgs      args                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( IsNull ( item ) ) item = CurrentItem ( )                              ;
  ////////////////////////////////////////////////////////////////////////////
  if ( hasFunction ( "Editable" ) )                                          {
    mm      . add          ( CiosMenuId(89,44,14) , tr("Open gallery") )     ;
    mm      . add          ( CiosMenuId(89,44,11) , tr("All pictures") )     ;
    mm      . add          ( CiosMenuId(89,44,12) , tr("Refresh"     ) )     ;
    mm      . addSeparator (                                           )     ;
    mm      . add          ( CiosMenuId(89,44,13) , tr("New gallery" ) )     ;
    mm      . addSeparator (                                           )     ;
    me = mm . addMenu      ( tr("Edit")                                )     ;
    mm      . add          ( me                                              ,
                             CiosMenuId(89,44,41)                            ,
                             tr("Count pictures in all galleries")         ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( hasFunction ( "Selectable" ) && NotNull ( item ) )                    {
    md = mm . addMenu      ( tr("Details")                                 ) ;
    mc = mm . addMenu      ( tr("Categorize")                              ) ;
    NameEditor             ( mm , md , item , &line                        ) ;
    MountUuid              ( mm , md , item                                ) ;
    mm      . addSeparator ( md                                            ) ;
    mm      . addSeparator (                                               ) ;
    //////////////////////////////////////////////////////////////////////////
    mm      . add          ( md                                              ,
                             CiosMenuId(89,44,51)                            ,
                             tr("Count pictures in this gallery")          ) ;
    mm      . addSeparator ( md                                            ) ;
    mm      . add          ( md                                              ,
                             CiosMenuId(89,44,71)                            ,
                             tr("Delete this gallery")                     ) ;
    mm      . add          ( md                                              ,
                             CiosMenuId(89,44,72)                            ,
                             tr("Delete all pictures in this gallery")     ) ;
    //////////////////////////////////////////////////////////////////////////
    mm      . add          ( mc , CiosMenuId(89,44,81) , tr("Constraints") ) ;
    mm      . add          ( mc , CiosMenuId(89,44,82) , tr("Rule tables") ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  ma   = mm . addMenu  ( tr("Adjustments")    )                              ;
  ms   = mm . addMenu  ( tr("Sorting")        )                              ;
  AdjustMenu           ( mm , ma              )                              ;
  SortingMenu          ( mm , ms              )                              ;
  ////////////////////////////////////////////////////////////////////////////
  mm . addSeparator    (                      )                              ;
  mm . add             ( CiosMenuId(89,44,99)                                ,
                         tr("Close galleries explorer")                    ) ;
  ////////////////////////////////////////////////////////////////////////////
  RunPickItemMenu                                                            ;
  if ( IsNull ( aa ) )                                                       {
    ModifyName ( item , line )                                               ;
    return true                                                              ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( AdjustProcessor  ( mm , aa        ) ) return true                     ;
  if ( SortingProcessor ( mm , aa        ) ) return true                     ;
  if ( 0 != CallToMenu  ( mm , aa , true ) ) return true                     ;
  ////////////////////////////////////////////////////////////////////////////
  switch ( mm [ aa ] )                                                       {
    case CiosMenuId    ( 89 , 44 , 99 )                                      : // Close this gallery explorer
      emit CloseObject ( node         )                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 11 )                                      : // View all pictures
      AllPictures      (              )                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 12 )                                      : // Refresh
      startup          (              )                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 13 )                                      : // New gallery
      start            ( CiosMenuId ( 85 , 12 , 28 ) )                       ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 14 )                                      : // Open gallery
      if ( NotNull ( item ) )                                                {
        emit DoubleClicked ( item )                                          ;
        atDoubleClicked    ( item )                                          ;
      }                                                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 41 )                                      : // Count pictures in all galleries
      start            ( CiosMenuId ( 85 , 12 , 26 ) )                       ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 51 )                                      : // Count pictures in this gallery
      if ( NotNull ( item ) )                                                {
        SUID itemUuid = item -> Value ( "UUID" ) . toULongLong ( )           ;
        args << item -> node                                                 ;
        args << itemUuid                                                     ;
        start          ( CiosMenuId ( 85 , 12 , 27 ) , args )                ;
      }                                                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 71 )                                      : // Delete this gallery
      if ( NotNull ( item ) )                                                {
        SUID itemUuid = item -> Value ( "UUID" ) . toULongLong ( )           ;
        args << item -> node                                                 ;
        args << itemUuid                                                     ;
        start          ( CiosMenuId ( 85 , 12 , 29 ) , args )                ;
      }                                                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 72 )                                      : // Delete all pictures in this gallery
      if ( NotNull ( item ) )                                                {
        SUID itemUuid = item -> Value ( "UUID" ) . toULongLong ( )           ;
        args << item -> node                                                 ;
        args << itemUuid                                                     ;
        start          ( CiosMenuId ( 85 , 12 , 30 ) , args )                ;
      }                                                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 81 )                                      : // Gallery categorize constraints
      if ( NotNull ( item ) )                                                {
        SUID itemUuid = item -> Value ( "UUID" ) . toULongLong ( )           ;
        args << item -> node                                                 ;
        args << itemUuid                                                     ;
        start          ( 73007 , args )                                      ;
      }                                                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 82 )                                      : // Gallery categorize rule tables
      if ( NotNull ( item ) )                                                {
        SUID itemUuid = item -> Value ( "UUID" ) . toULongLong ( )           ;
        args << item -> node                                                 ;
        args << itemUuid                                                     ;
        start          ( 73008 , args )                                      ;
      }                                                                      ;
    break                                                                    ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

#endif
