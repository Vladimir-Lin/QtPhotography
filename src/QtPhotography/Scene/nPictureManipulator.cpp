#include <qtphotography.h>

#ifdef QT_OPENGL_LIB

N::PictureManipulator:: PictureManipulator ( QObject * parent , Plan * p )
                      : PictureObject      (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PictureManipulator:: PictureManipulator ( QObject * parent , Plan * p , SceneDestructor * des )
                      : PictureObject      (           parent ,        p ,                   des )
{
  Configure ( ) ;
}

N::PictureManipulator::~PictureManipulator (void)
{
}

void N::PictureManipulator::Configure(void)
{
  ControlPoint color                                                ;
  color  = QColor       ( 192 ,  192 , 192 , 192                  ) ;
  addColor              (    0 , color                            ) ;
  color  = ControlPoint (    0 ,    0 ,   1 , 0.01                ) ;
  addColor              (    1 , color                            ) ;
  color  = ControlPoint (    1 ,    0 ,   0 , 0.01                ) ;
  addColor              (    2 , color                            ) ;
  color  = ControlPoint (    0 ,  0.6 ,   0 , 0.01                ) ;
  addColor              (    3 , color                            ) ;
  color  = ControlPoint (    1 ,    1 ,   1 , 0.15                ) ;
  addColor              (    4 , color                            ) ;
  color  = ControlPoint ( 0.10 , 0.70 ,   1 , 0.03                ) ;
  addColor              (    5 , color                            ) ;
  color  = ControlPoint ( 0.20 , 0.75 , 0.2 , 0.10                ) ;
  addColor              (    6 , color                            ) ;
  color  = ControlPoint ( 1.00 , 1.00 , 0.9 , 0.15                ) ;
  addColor              (    7 , color                            ) ;
  color  = ControlPoint ( 1.00 , 1.00 , 0.9 , 0.15                ) ;
  addColor              (    8 , color                            ) ;
  color  = ControlPoint ( 0.70 , 0.25 , 0.5 , 0.25                ) ;
  addColor              (    9 , color                            ) ;
  color  = ControlPoint ( 0.80 , 0.75 , 0.6 , 0.10                ) ;
  addColor              (   10 , color                            ) ;
  color  = ControlPoint ( 0.60 , 0.80 , 0.9 , 0.10                ) ;
  addColor              (   11 , color                            ) ;
  color  = ControlPoint ( 0.50 , 0.70 , 0.3 , 0.25                ) ;
  addColor              (   12 , color                            ) ;
  ///////////////////////////////////////////////////////////////////
  QObject::setProperty  ( "Type"           , "PictureManipulator" ) ;
  setVariable           ( "Type"           , "PictureManipulator" ) ;
  ///////////////////////////////////////////////////////////////////
  setVariable           ( "Focus"          , false                ) ;
  setVariable           ( "Hold"           , false                ) ;
  setVariable           ( "Menu"           , true                 ) ;
  setVariable           ( "Mouse"          , true                 ) ;
  setVariable           ( "Hover"          , true                 ) ;
  setVariable           ( "Emit"           , true                 ) ;
  setVariable           ( "Finished"       , false                ) ;
  setVariable           ( "Finalize"       , false                ) ;
  ///////////////////////////////////////////////////////////////////
  setVariable           ( "Selectable"     , false                ) ;
  setVariable           ( "Movable"        , true                 ) ;
  setVariable           ( "Resizable"      , true                 ) ;
  setVariable           ( "Margin"         , 3                    ) ;
  ///////////////////////////////////////////////////////////////////
  setVariable           ( "SelectionColor" , 4                    ) ;
  ///////////////////////////////////////////////////////////////////
  ControlPoint at       ( 0 , 0 , 0 , 1                           ) ;
  ControlPoint zp       ( 0 , 0 , 0 , 1                           ) ;
  ControlPoint scale    ( 1 , 1 , 1 , 1                           ) ;
  ControlPoint rotate   ( 0 , 0 , 1 , 0                           ) ;
  ///////////////////////////////////////////////////////////////////
  at      . DOF = ControlPoint::DofTranslate                        ;
  zp      . DOF = ControlPoint::DofTranslate                        ;
  scale   . DOF = ControlPoint::DofScale                            ;
  rotate  . DOF = ControlPoint::DofRotate                           ;
  ///////////////////////////////////////////////////////////////////
  addDof                ( 0 , at                                  ) ;
  addDof                ( 1 , zp                                  ) ;
  addDof                ( 2 , scale                               ) ;
  addDof                ( 3 , rotate                              ) ;
}

bool N::PictureManipulator::FocusIn(void)
{
  nKickOut   ( IsNull ( plan ) , false       ) ;
  LinkAction ( ZoomIn          , ZoomIn  ( ) ) ;
  LinkAction ( ZoomOut         , ZoomOut ( ) ) ;
  return true                                  ;
}

bool N::PictureManipulator::FocusOut(void)
{
  return false ;
}

int N::PictureManipulator::AtRegion(ControlPoint & At)
{
  return RectRegion ( At ) ;
}

bool N::PictureManipulator::Spot (
       ControlPoint & From       ,
       ControlPoint & To         ,
       ControlPoint & At         )
{
  if (   Value ( "Finalize" ) . toBool ( ) ) return false ;
  if ( ! Value ( "Mouse"    ) . toBool ( ) ) return false ;
  return xymSpot ( From , To , At )                       ;
}

bool N::PictureManipulator::MouseClicked (
       ControlPoint & From               ,
       ControlPoint & To                 ,
       BMAPs        & buttons            )
{
  if (   Value ( "Finalize" ) . toBool ( ) ) return false ;
  if ( ! Value ( "Mouse"    ) . toBool ( ) ) return false ;
  if ( ! matrices . contains ( 2 )         ) return false ;
  if ( buttons [ Universe::MouseMiddle ]   ) return false ;
  if ( buttons [ Universe::MouseRight  ]   ) return false ;
  /////////////////////////////////////////////////////////
  ControlPoint At                                         ;
  if ( ! Collide ( From , To , At )        ) return false ;
  /////////////////////////////////////////////////////////
  if ( buttons [ Universe::MouseLeft   ] )                {
    if ( ! isFocus ( ) )                                  {
      Alert ( Click )                                     ;
    }                                                     ;
    setFocus ( true )                                     ;
    setVariable  ( "Hit" , true    )                      ;
  } else                                                  {
    if ( Value ( "Hit" ) . toBool ( ) )                   {
      setVariable      ( "Hit" , false   )                ;
    }                                                     ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  return true                                             ;
}

void N::PictureManipulator::BoxChanged(void)
{
}

void N::PictureManipulator::FinalizeSelection(void)
{
}

bool N::PictureManipulator::MouseVector (
       ControlPoint & From              ,
       ControlPoint & To                ,
       BMAPs        & buttons           )
{
  if (   Value ( "Finalize" ) . toBool ( ) ) return false  ;
  if ( ! Value ( "Mouse"    ) . toBool ( ) ) return false  ;
  if ( ! matrices . contains ( 2 )         ) return false  ;
  if ( buttons [ Universe::MouseMiddle ]   ) return false  ;
  if ( buttons [ Universe::MouseRight  ]   ) return false  ;
  //////////////////////////////////////////////////////////
  ControlPoint At                                          ;
  if ( ! Collide ( From , To , At )        ) return false  ;
  return MouseEditing ( From , To , At , buttons , false ) ;
}

bool N::PictureManipulator::MouseHover (
       ControlPoint & From             ,
       ControlPoint & To               ,
       BMAPs        & buttons          )
{
  if (   Value ( "Finalize" ) . toBool ( ) ) return false     ;
  if ( ! Value ( "Mouse"    ) . toBool ( ) ) return false     ;
  if ( ! isFocus             (   )         ) return false     ;
  if ( ! matrices . contains ( 2 )         ) return false     ;
  /////////////////////////////////////////////////////////////
  ControlPoint At                                             ;
  bool         grab    = Value   ( "Grab" ) . toBool ( )      ;
  bool         collide = Collide ( From , To , At )           ;
  if ( ( ! grab ) && ( ! collide ) ) return false             ;
  QPointF AT  ( At . x , At . y   )                           ;
  setVariable ( "CurrentPos" , AT )                           ;
  /////////////////////////////////////////////////////////////
  if ( MouseEditing ( From , To , At , buttons , true ) )     {
    return true                                               ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  int region = AtRegion ( At )                                ;
  if ( region <= 0 )                                          {
    setCursor ( Qt::ArrowCursor )                             ;
    return false                                              ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  bool onlyMoving = true                                      ;
  if ( buttons [ Universe::MouseLeft   ] ) onlyMoving = false ;
  if ( buttons [ Universe::MouseMiddle ] ) onlyMoving = false ;
  if ( buttons [ Universe::MouseRight  ] ) onlyMoving = false ;
  if ( ! onlyMoving ) return true                             ;
  /////////////////////////////////////////////////////////////
  AssignCursor ( region )                                     ;
  /////////////////////////////////////////////////////////////
  return true                                                 ;
}

bool N::PictureManipulator::MouseMove (
       ControlPoint & From1           ,
       ControlPoint & To1             ,
       ControlPoint & From2           ,
       ControlPoint & To2             ,
       BMAPs        & buttons         )
{
  return false ;
}

QString N::PictureManipulator::ToolTip(void)
{
  QImage * image = CurrentImage ( )                               ;
  if ( IsNull(image) ) return ""                                  ;
  /////////////////////////////////////////////////////////////////
  QPointF p  = Value ( "CurrentPos" ) . toPointF ( )              ;
  QSize   s  = image -> size (   )                                ;
  QString AS                                                      ;
  QRect   RX                                                      ;
  double  xx = p . x ( ) - minimum . x                            ;
  double  yy = p . y ( ) - minimum . y                            ;
  /////////////////////////////////////////////////////////////////
  xx /= length . x                                                ;
  yy /= length . y                                                ;
  yy  = 1.0 - yy                                                  ;
  xx *= s . width  ( )                                            ;
  yy *= s . height ( )                                            ;
  AS  = tr("<%1,%2>").arg(xx).arg(yy)                             ;
  AS  . append ( "\n" )                                           ;
  AS += tr("%1 x %2").arg(s.width()).arg(s.height())              ;
  /////////////////////////////////////////////////////////////////
  if ( AS . length ( ) > 0 )                                      {
    QToolTip::showText ( QCursor::pos() , AS , NULL , RX , 3000 ) ;
    if ( NULL != gui ) gui -> setToolTip ( AS )                   ;
  }                                                               ;
  return AS                                                       ;
}

void N::PictureManipulator::renderFaces(void)
{
  PictureObject::renderFaces ( ) ;
  renderSelectRect           ( ) ;
}

void N::PictureManipulator::run(int type,ThreadData * data)
{
  switch ( type )                                                {
    case 10001                                                   :
      FetchTexture ( data -> Arguments [ 0 ] . toULongLong ( ) ) ;
    break                                                        ;
  }                                                              ;
}

QImage * N::PictureManipulator::FetchPicture(SqlConnection & SC,SUID u)
{
  QImage       * t = NULL       ;
  PictureManager PM ( plan )    ;
  if ( u > 0 )                  {
    t = PM . Picture ( SC , u ) ;
  }                             ;
  ///////////////////////////////
  return t                      ;
}

void N::PictureManipulator::FetchTexture(SUID u)
{
  QImage      * img = NULL                      ;
  SqlConnection SC ( plan -> sql )              ;
  ///////////////////////////////////////////////
  if ( SC . open ( FunctionString ) )           {
    img = FetchPicture ( SC , u )               ;
    SC . close  ( )                             ;
  }                                             ;
  SC   . remove ( )                             ;
  ///////////////////////////////////////////////
  if ( IsNull ( img ) ) return                  ;
  maximum . x = img -> width  ( ) + minimum . x ;
  maximum . y = img -> height ( ) + minimum . y ;
  Finish     (                )                 ;
  Dual       (                )                 ;
  setTexture ( u , img , true )                 ;
}

bool N::PictureManipulator::LoadTexture(SUID uuid)
{
  VarArgs args           ;
  args << uuid           ;
  start ( 10001 , args ) ;
  return true            ;
}

void N::PictureManipulator::ZoomIn(void)
{
  if ( ! Value ( "Resizable" ) . toBool ( ) ) return ;
  ControlPoint z = DOFs [ 2 ]                        ;
  z *= 1.1                                           ;
  LockAt   ( "Operate" )                             ;
  DOFs [ 2 ] . x = z . x                             ;
  DOFs [ 2 ] . y = z . y                             ;
  Finish   (           )                             ;
  Dual     (           )                             ;
  UnlockAt ( "Operate" )                             ;
}

void N::PictureManipulator::ZoomOut(void)
{
  if ( ! Value ( "Resizable" ) . toBool ( ) ) return ;
  ControlPoint z = DOFs [ 2 ]                        ;
  z *= 0.9                                           ;
  LockAt   ( "Operate" )                             ;
  DOFs [ 2 ] . x = z . x                             ;
  DOFs [ 2 ] . y = z . y                             ;
  Finish   (           )                             ;
  Dual     (           )                             ;
  UnlockAt ( "Operate" )                             ;
}

void N::PictureManipulator::NormalRatio(void)
{
  QImage * image = CurrentImage ( )          ;
  if ( IsNull(image) ) return                ;
  ////////////////////////////////////////////
  QSize s  = image -> size (   )             ;
  LockAt   ( "Operate" )                     ;
  maximum . x = minimum . x + s . width  ( ) ;
  maximum . y = minimum . y + s . height ( ) ;
  DOFs [ 2 ] . x = 1                         ;
  DOFs [ 2 ] . y = 1                         ;
  DOFs [ 2 ] . z = 1                         ;
  DOFs [ 2 ] . t = 1                         ;
  Finish   (           )                     ;
  Dual     (           )                     ;
  UnlockAt ( "Operate" )                     ;
}

void N::PictureManipulator::PlaceCentral(void)
{
  QImage * image = CurrentImage ( )          ;
  if ( IsNull(image) ) return                ;
  ////////////////////////////////////////////
  QSize  s = image -> size (   )             ;
  double x                                   ;
  double y                                   ;
  LockAt   ( "Operate" )                     ;
  maximum . x = minimum . x + s . width  ( ) ;
  maximum . y = minimum . y + s . height ( ) ;
  x           = minimum . x + maximum . x    ;
  y           = minimum . x + maximum . x    ;
  x          /= 2                            ;
  y          /= 2                            ;
  minimum . x    -= x                        ;
  minimum . y    -= y                        ;
  maximum . x    -= x                        ;
  maximum . y    -= y                        ;
  DOFs [ 0 ] . x += x                        ;
  DOFs [ 0 ] . y += y                        ;
  DOFs [ 2 ] . x  = 1                        ;
  DOFs [ 2 ] . y  = 1                        ;
  DOFs [ 2 ] . z  = 1                        ;
  DOFs [ 2 ] . t  = 1                        ;
  Finish   (           )                     ;
  Dual     (           )                     ;
  UnlockAt ( "Operate" )                     ;
}

void N::PictureManipulator::AdjustMenu(MenuManager & mm,QMenu * ma)
{
  mm . add ( ma                       ,
             320001                   ,
             tr("1:1 ratio")        ) ;
  mm . add ( ma                       ,
             320002                   ,
             tr("Central mode")     ) ;
  mm . addSeparator ( ma )            ;
  SceneWidget::AdjustMenu ( mm , ma ) ;
}

bool N::PictureManipulator::AdjustProcessor(MenuManager & mm,QAction * aa)
{
  switch ( mm [ aa ] )                              {
    case 320001                                     :
      NormalRatio  (                              ) ;
    return true                                     ;
    case 320002                                     :
      PlaceCentral (                              ) ;
    return true                                     ;
    default                                         :
    return SceneWidget::AdjustProcessor ( mm , aa ) ;
  }                                                 ;
  return false                                      ;
}

bool N::PictureManipulator::Menu (
       QWidget      * widget     ,
       QPointF        pos        ,
       ControlPoint & From       ,
       ControlPoint & To         )
{
  if ( ! Value ( "Mouse" ) . toBool ( ) ) return false                       ;
  setCursor ( Qt::ArrowCursor )                                              ;
  ////////////////////////////////////////////////////////////////////////////
  ControlPoint At                                                            ;
  if ( ! Collide ( From , To , At ) ) return false                           ;
  ////////////////////////////////////////////////////////////////////////////
  MenuManager mm ( widget )                                                  ;
  QMenu     * ma                                                             ;
  QMenu     * me                                                             ;
  QAction   * aa                                                             ;
  ////////////////////////////////////////////////////////////////////////////
  ma  = mm . addMenu   ( tr("Adjustments")    )                              ;
  AdjustMenu           ( mm , ma              )                              ;
  ////////////////////////////////////////////////////////////////////////////
  mm . addSeparator    (                      )                              ;
  mm . add             ( 901 , tr("Close")    )                              ;
  ////////////////////////////////////////////////////////////////////////////
  mm  . setFont        ( plan                 )                              ;
  aa  = mm . exec      (                      )                              ;
  nKickOut             ( IsNull ( aa ) , true )                              ;
  ////////////////////////////////////////////////////////////////////////////
  if ( AdjustProcessor ( mm , aa ) ) return true                             ;
  ////////////////////////////////////////////////////////////////////////////
  switch ( mm [ aa ] )                                                       {
    case 901                                                                 :
      emit CloseObject ( node )                                              ;
    break                                                                    ;
  }                                                                          ;
  return true                                                                ;
}

#endif
