#include <qtphotography.h>

typedef struct     {
  N::Plan * plan   ;
  QWidget * widget ;
} ExtraPacket      ;

N::PicturesDispatcher:: PicturesDispatcher ( QObject * parent )
                      : QObject            (           parent )
                      , SyntaxDispatcher   (                  )
                      , UuidSyntax         (                  )
                      , plan               ( NULL             )
{
  Variables [ "SQL" ] = false                    ;
  nConnect ( this , SIGNAL ( ViewPicture ( ) )   ,
             this , SLOT   ( SendPicture ( ) ) ) ;
}

N::PicturesDispatcher::~PicturesDispatcher (void)
{
}

int N::PicturesDispatcher::type(void) const
{
  return CiosMenuId ( 77 , 0 , 1 ) ;
}

void N::PicturesDispatcher::Help(QStringList & help)
{
  help << tr("===== Pictures dispatcher commands ====="  ) ;
  help << QString("set pictures variable name value : %1").arg(tr("set variable into name")) ;
  help << QString("show pictures variables : %1"         ).arg(tr("show variables")) ;
  help << QString("find pictures from sql : %1"          ).arg(tr("enable sql query")) ;
  help << QString("stop pictures sql : %1"               ).arg(tr("Stop sql query")) ;
  help << QString("select ... : %1"                      ).arg(tr("Select pictures from sql")) ;
  help << QString("build pictures for hash : %1"         ).arg(tr("build adler32, md5, sha1 for pictures")) ;
  help << QString("build pictures for missing : %1"      ).arg(tr("check lost pictures in database")) ;
  help << QString("search pictures by tags ... syntax ... : %1").arg(tr("Using tags to search pictures")) ;
}

int N::PicturesDispatcher::matching(QString cmd)
{
  QStringList C = CommandTokens ( cmd )                                      ;
  command    = cmd                                                           ;
  arguments  . clear ( )                                                     ;
  arguments <= C                                                             ;
  if ( Variables [ "SQL" ] . toBool ( ) )                                    {
    if ( ( C . count ( ) > 1 ) && ( "stop" == C [ 0 ] . toLower ( ) )      ) {
      if ( "pictures" == C [ 1 ] . toLower ( )                             ) {
        if ( "sql" == C [ 2 ] . toLower ( )                                ) {
          Report ( tr("Stop all SQL query") )                                ;
          Report ( "\n"                     )                                ;
          return CiosMenuId ( 77 , 1 , 2 )                                   ;
        }                                                                    ;
      }                                                                      ;
    } else
    if ( ( C . count ( ) > 1 ) && ( "select" == C [ 0 ] . toLower ( ) )    ) {
      Report ( tr("SQL query command") )                                     ;
      Report ( " => "                  )                                     ;
      Report ( command                 )                                     ;
      Report ( "\n"                    )                                     ;
      return CiosMenuId ( 77 , 2 , 1 )                                       ;
    }                                                                        ;
  } else                                                                     {
    if ( ( C . count ( ) > 1 ) && ( "find" == C [ 0 ] . toLower ( ) )      ) {
      if ( "pictures" == C [ 1 ] . toLower ( )                             ) {
        if ( ( C . count ( ) > 2 ) && ( "from" == C [ 2 ] . toLower ( ) )  ) {
          if ( ( C . count ( ) > 3 ) && ( "sql" == C [ 3 ] . toLower ( ) ) ) {
            Report ( tr("Enter SQL query for pictures") )                    ;
            Report ( "\n"                               )                    ;
            return CiosMenuId ( 77 , 1 , 1 )                                 ;
          }                                                                  ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( ( C . count ( ) == 5 ) && ( "set" == C [ 0 ] . toLower ( ) )      ) {
      if ( "pictures" == C [ 1 ] . toLower ( )                             ) {
        if ( ( "variable" == C [ 2 ] . toLower ( ) )  )                      {
          return CiosMenuId ( 77 , 2 , 2 )                                   ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( ( C . count ( ) == 3 ) && ( "show" == C [ 0 ] . toLower ( ) )     ) {
      if ( "pictures" == C [ 1 ] . toLower ( )                             ) {
        if ( ( "variables" == C [ 2 ] . toLower ( ) )  )                     {
          return CiosMenuId ( 77 , 2 , 3 )                                   ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( ( C . count ( ) > 1 ) && ( "build" == C [ 0 ] . toLower ( ) )     ) {
      if ( "pictures" == C [ 1 ] . toLower ( )                             ) {
        if ( ( C . count ( ) > 2 ) && ( "for" == C [ 2 ] . toLower ( ) )  )  {
          if ( C . count ( ) > 3 )                                           {
            if ( "hash"    == C [ 3 ] . toLower ( ) )                        {
              return CiosMenuId ( 77 , 3 , 1 )                               ;
            }                                                                ;
            if ( "missing" == C [ 3 ] . toLower ( ) )                        {
              return CiosMenuId ( 77 , 3 , 2 )                               ;
            }                                                                ;
          }                                                                  ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( ( C . count ( ) > 4 ) && ( "search" == C [ 0 ] . toLower ( ) ) )    {
      if ( "pictures" == C [ 1 ] . toLower ( ) )                             {
        if ( "by" == C [ 2 ] . toLower ( ) )                                 {
          if ( "tags"  == C [ 3 ] . toLower ( ) )                            {
            return CiosMenuId ( 77 , 4 , 1 )                                 ;
          }                                                                  ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
  }                                                                          ;
  return 0                                                                   ;
}

bool N::PicturesDispatcher::execute(int id)
{
  switch ( id )                                     {
    case CiosMenuId ( 77 , 1 , 1 )                  :
      Variables [ "SQL" ] = true                    ;
    return true                                     ;
    case CiosMenuId ( 77 , 1 , 2 )                  :
      Variables [ "SQL" ] = false                   ;
    return true                                     ;
    case CiosMenuId ( 77 , 2 , 1 )                  :
      FindPictures  (            )                  ;
    return true                                     ;
    case CiosMenuId ( 77 , 2 , 2 )                  :
      setVariable   (            )                  ;
    return true                                     ;
    case CiosMenuId ( 77 , 2 , 3 )                  :
      showVariables (            )                  ;
    return true                                     ;
    case CiosMenuId ( 77 , 3 , 1 )                  :
      BuildHash     (            )                  ;
    return true                                     ;
    case CiosMenuId ( 77 , 3 , 2 )                  :
      PictureUUIDs  (            )                  ;
    return true                                     ;
    case CiosMenuId ( 77 , 4 , 1 )                  :
      SearchTags    (            )                  ;
    return true                                     ;
  }                                                 ;
  return false                                      ;
}

bool N::PicturesDispatcher::FetchByTag(SqlConnection & SC,UUIDs & U,QString Key)
{
  TagsManager TM ( plan )                         ;
  GroupItems  GI ( plan )                         ;
  UUIDs       T                                   ;
  UUIDs       P                                   ;
  UUIDs       L                                   ;
  SUID        u                                   ;
  /////////////////////////////////////////////////
  TM . GetTags ( SC , T , Key )                   ;
  if ( T . count ( ) <= 0 ) return false          ;
  /////////////////////////////////////////////////
  GI . AutoMap    = true                          ;
  GI . GroupTable = GI . LookTable                (
                      Types::Tag                  ,
                      Types::Picture              ,
                      Groups::Subordination     ) ;
  foreach ( u , T )                               {
    P = GI . Subordination                        (
          SC                                      ,
          u                                       ,
          Types::Tag                              ,
          Types::Picture                          ,
          Groups::Subordination                   ,
          SC . OrderByAsc ( "position" )        ) ;
    L << P                                        ;
  }                                               ;
  /////////////////////////////////////////////////
  P . clear      (   )                            ;
  P = Uniqueness ( L )                            ;
  foreach ( u , P )                               {
    if ( ! U . contains ( u ) ) U << u            ;
  }                                               ;
  /////////////////////////////////////////////////
  return ( U . count ( ) > 0 )                    ;
}

void N::PicturesDispatcher::setExtras(void * data)
{
  ExtraPacket * ep = (ExtraPacket *) data                              ;
  QWidget     * ww = ep -> widget                                      ;
  plan = ep -> plan                                                    ;
  QObject::connect ( this , SIGNAL ( Pictures     (QString,UUIDs&) )   ,
                     ww   , SLOT   ( ViewPictures (QString,UUIDs&) ) ) ;
}

void N::PicturesDispatcher::SendPicture(void)
{
  while ( PictureQueues . count ( ) > 0 )  {
    UUIDs U = PictureQueues . first ( )    ;
    PictureQueues . takeFirst ( )          ;
    emit Pictures ( tr("SQL search") , U ) ;
  }                                        ;
}

void N::PicturesDispatcher::FindPictures(void)
{
  if ( arguments . count ( ) > 1 )                             {
    QString T                                                  ;
    T = arguments [ 1 ] . toString ( ) . toLower ( )           ;
    if ( ( T != "uuid" ) && ( T != "`uuid`" )                ) {
      Report ( tr("You are not trying to find pictures") )     ;
      Report ( "\n"                    )                       ;
      return                                                   ;
    }                                                          ;
  }                                                            ;
  //////////////////////////////////////////////////////////////
  UUIDs U                                                      ;
  SqlConnection SC   ( plan -> sql    )                        ;
  if ( SC . open     ( FunctionString )                      ) {
    SqlLoopNow       ( SC , command   )                        ;
      U << SC . Uuid ( 0              )                        ;
    SqlLoopErr       ( SC , command   )                        ;
    SqlLoopEnd       ( SC , command   )                        ;
    SC . close       (                )                        ;
  }                                                            ;
  SC   . remove      (                )                        ;
  //////////////////////////////////////////////////////////////
  if ( U . count ( ) > 0 )                                     {
    Report ( tr("Found %1 pictures") . arg ( U . count ( ) ) ) ;
    Report ( "\n"                    )                         ;
    PictureQueues << U                                         ;
    emit ViewPicture ( )                                       ;
  } else                                                       {
    Report ( tr("Found nothing") )                             ;
    Report ( "\n"                )                             ;
  }                                                            ;
}

void N::PicturesDispatcher::setVariable(void)
{
  if ( arguments . count ( ) != 5 ) return       ;
  QString n = arguments [ 3 ] . toString ( )     ;
  QString v = arguments [ 4 ] . toString ( )     ;
  Variables [ n ] = arguments [ 4 ]              ;
  Report ( QString("%1 => %2\n").arg(n).arg(v) ) ;
}

void N::PicturesDispatcher::showVariables(void)
{
  QStringList K = Variables . keys ( )             ;
  QString     k                                    ;
  QString     v                                    ;
  foreach ( k , K )                                {
    v = Variables [ k ] . toString ( )             ;
    Report ( QString("%1 => %2\n").arg(k).arg(v) ) ;
  }                                                ;
}

void N::PicturesDispatcher::BuildHash(void)
{
  SqlConnection SC ( plan -> sql )                                           ;
  if ( SC . open ( FunctionString ) )                                        {
    QString Q                                                                ;
    QString T                                                                ;
    UUIDs   U                                                                ;
    UUIDs   R                                                                ;
    SUID    u                                                                ;
    qint64  total = 0                                                        ;
    qint64  index = 0                                                        ;
    bool    go    = true                                                     ;
    int     id    = -1                                                       ;
    //////////////////////////////////////////////////////////////////////////
    T = PlanTable(PictureUuid)                                               ;
    Q = QString("select `uuid` from %1 order by `id` asc ;").arg(T)          ;
    SqlLoopNow ( SC , Q )                                                    ;
      U << SC . Uuid ( 0 )                                                   ;
    SqlLoopErr ( SC , Q )                                                    ;
    SqlLoopEnd ( SC , Q )                                                    ;
    //////////////////////////////////////////////////////////////////////////
    Q = QString("select uuid from `picturesummary` order by `id` asc ;")     ;
    SqlLoopNow ( SC , Q )                                                    ;
      R << SC . Uuid ( 0 )                                                   ;
    SqlLoopErr ( SC , Q )                                                    ;
    SqlLoopEnd ( SC , Q )                                                    ;
    //////////////////////////////////////////////////////////////////////////
    U     = U - R                                                            ;
    total = U . count ( )                                                    ;
    id    = plan -> Progress ( tr("Building MD5") , tr("%v/%m") )            ;
    plan         -> Start    ( id , &index , &go                )            ;
    plan         -> setRange ( id , 0      , total              )            ;
    for (qint64 i = 0 ; go && ( i < total ) ; i++ , index ++ )               {
      u  = U [ i ]                                                           ;
      Q  = SC . sql . SelectFrom                                             (
            "file"                                                           ,
            T                                                                ,
            SC . WhereUuid ( u )                                           ) ;
      if ( SC . Fetch ( Q ) )                                                {
        QByteArray P = SC . ByteArray ( 0 )                                  ;
        if ( P . size ( ) > 0 )                                              {
          unsigned int adler = ADLER32 ( P )                                 ;
          QByteArray   md5   = QCryptographicHash::hash ( P , QCryptographicHash::Md5  ) ;
          QByteArray   sha1  = QCryptographicHash::hash ( P , QCryptographicHash::Sha1 ) ;
          md5  = md5  . toHex ( )                                            ;
          sha1 = sha1 . toHex ( )                                            ;
          Q = SC . sql . ReplaceInto                                         (
                PlanTable(PictureSummary)                                    ,
                4                                                            ,
                "uuid"                                                       ,
                "adler32"                                                    ,
                "md5"                                                        ,
                "sha1"                                                     ) ;
          SC . Prepare ( Q                 )                                 ;
          SC . Bind    ( "uuid"    , u     )                                 ;
          SC . Bind    ( "adler32" , adler )                                 ;
          SC . Bind    ( "md5"     , md5   )                                 ;
          SC . Bind    ( "sha1"    , sha1  )                                 ;
          SC . Exec    (                   )                                 ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    plan         -> Finish   ( id                               )            ;
    //////////////////////////////////////////////////////////////////////////
    SC . close  ( )                                                          ;
  }                                                                          ;
  SC   . remove ( )                                                          ;
  Alert ( Done )                                                             ;
}

void N::PicturesDispatcher::PictureUUIDs(void)
{
  SqlConnection SC ( plan -> sql )                                           ;
  if ( SC . open ( FunctionString ) )                                        {
    QString Q                                                                ;
    UUIDs   P                                                                ;
    UUIDs   U                                                                ;
    UUIDs   M                                                                ;
    UUIDs   O                                                                ;
    UUIDs   L                                                                ;
    SUID    u                                                                ;
    P = SC . Uuids                                                           (
          PlanTable(PictureUuid)                                             ,
          "uuid"                                                             ,
          SC . OrderByAsc ( "id" )                                         ) ;
    U = SC . Uuids                                                           (
          PlanTable(PictureOrders)                                           ,
          "uuid"                                                             ,
          SC . OrderByAsc ( "id" )                                         ) ;
    M = SC . Uuids                                                           (
          PlanTable(PictureOrders)                                           ,
          "uuid"                                                             ,
          QString ( "where type = %1 %2"                                     )
          . arg   ( Types::Picture                                           )
          . arg   ( SC . OrderByAsc ( "id" ) )                             ) ;
    //////////////////////////////////////////////////////////////////////////
    O = P - U                                                                ;
    L = M - U                                                                ;
    //////////////////////////////////////////////////////////////////////////
    if ( O . count ( ) > 0 )                                                 {
      foreach ( u , O )                                                      {
        Q = SC . sql . InsertInto                                            (
              "uuid"                                                         ,
              PlanTable(PictureOrders)                                       ,
              QString::number(u)                                           ) ;
        SC . Query ( Q    )                                                  ;
        Report     ( Q    )                                                  ;
        Report     ( "\n" )                                                  ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( L . count ( ) > 0 )                                                 {
      foreach ( u , L )                                                      {
        Q = SC . sql . DeleteFrom ( PlanTable(MajorUuid) , SC.WhereUuid(u) ) ;
        SC . Query ( Q    )                                                  ;
        Report     ( Q    )                                                  ;
        Report     ( "\n" )                                                  ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    SC  . close  (      )                                                    ;
  }                                                                          ;
  SC    . remove (      )                                                    ;
  Alert          ( Done )                                                    ;
}

void N::PicturesDispatcher::SearchTags(void)
{
  QStringList s = Syntax ( command )                    ;
  QStringList k                                         ;
  QStringList t                                         ;
  QString     l                                         ;
  bool        c = true                                  ;
  ///////////////////////////////////////////////////////
  l = s [ 0 ] . toLower ( )                             ;
  if ( "search"   != l ) c = false                      ;
  l = s [ 1 ] . toLower ( )                             ;
  if ( "pictures" != l ) c = false                      ;
  l = s [ 2 ] . toLower ( )                             ;
  if ( "by"       != l ) c = false                      ;
  l = s [ 3 ] . toLower ( )                             ;
  if ( "tags"     != l ) c = false                      ;
  ///////////////////////////////////////////////////////
  if ( ! c )                                            {
    Report ( tr("Syntax error") )                       ;
    Report ( "\n"               )                       ;
    return                                              ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  for (int i=0;i<4;i++) s . takeFirst ( )               ;
  ///////////////////////////////////////////////////////
  int left  = 0                                         ;
  int right = 0                                         ;
  if ( ! isSyntax ( left , right , s , k ) )            {
    Report ( tr("Syntax error") )                       ;
    Report ( "\n"               )                       ;
    return                                              ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  for (int i=left;i<=right;i++) t << s [ i ]            ;
  l = t . join ( " " )                                  ;
  Report ( tr("Syntax : %1") . arg ( l )   )            ;
  Report ( "\n"                            )            ;
  ///////////////////////////////////////////////////////
  QMap<QString,UUIDs> UM                                ;
  UUIDs               X                                 ;
  SqlConnection SC   ( plan -> sql         )            ;
  if ( SC . open     ( FunctionString )    )            {
    UUIDs U                                             ;
    /////////////////////////////////////////////////////
    foreach          ( l , k               )            {
      UM [ l ] = U                                      ;
      FetchByTag     ( SC , UM [ l ] , l   )            ;
    }                                                   ;
    /////////////////////////////////////////////////////
    Operate ( U , t , UM )                              ;
    X = Uniqueness ( U )                                ;
    /////////////////////////////////////////////////////
    SC . close       (                     )            ;
  }                                                     ;
  SC   . remove      (                     )            ;
  ///////////////////////////////////////////////////////
  if ( X . count ( ) > 0 )                              {
    QString m                                           ;
    m = tr("Found %1 pictures") . arg ( X . count ( ) ) ;
    Report ( m    )                                     ;
    Report ( "\n" )                                     ;
    l = t . join ( " " )                                ;
    PictureQueues  << X                                 ;
    ListQueues [ l ] = X                                ;
    emit ViewPicture ( )                                ;
  } else                                                {
    Report ( tr("Found nothing") )                      ;
    Report ( "\n"                )                      ;
  }                                                     ;
}
