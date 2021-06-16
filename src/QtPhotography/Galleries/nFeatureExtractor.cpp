#include <qtphotography.h>

#pragma pack(push,1)

typedef struct   {
  double Hessian ;
  qint32 Octaves ;
  qint32 Layers  ;
} SurfParaments  ;

#pragma pack(pop)

N::FeatureExtractor:: FeatureExtractor ( QObject * parent , Plan * p )
                    : QObject          (           parent            )
                    , CommandSequence  ( new QTimer ( this )         )
                    , plan             (                           p )
                    , Commanding       ( false                       )
{
  Configure ( ) ;
}

N::FeatureExtractor::~FeatureExtractor (void)
{
}

void N::FeatureExtractor::Configure(void)
{
  CommandSequence -> setParent ( this )                        ;
  nConnect ( this            , SIGNAL ( SafeComplete   ( ) )   ,
             this            , SLOT   ( RelayComplete  ( ) ) ) ;
  nConnect ( this            , SIGNAL ( TriggerCommand ( ) )   ,
             this            , SLOT   ( StartCommand   ( ) ) ) ;
  nConnect ( CommandSequence , SIGNAL ( timeout        ( ) )   ,
             this            , SLOT   ( CommandHandler ( ) ) ) ;
}

void N::FeatureExtractor::RelayComplete(void)
{
  emit Completed ( ) ;
}

void N::FeatureExtractor::StartCommand(void)
{
  nDropOut ( Commanding )          ;
  CommandSequence -> start ( 300 ) ;
}

void N::FeatureExtractor::CommandHandler(void)
{
  nDropOut ( Commanding )         ;
  Commanding = true               ;
  while ( Sequences.count() > 0 ) {
    RunCommand ( Sequences[0] )   ;
    Sequences . takeAt ( 0 )      ;
  }                               ;
  CommandSequence -> stop ( )     ;
  Commanding = false              ;
}

bool N::FeatureExtractor::RunCommand(VarArgs & arguments)
{
  if (arguments.count()<1) return false ;
  VarArgs V = arguments                 ;
  int     c = V [ 0 ] . toInt ( )       ;
  switch         ( c     )              {
    case 3002                           :
      V . takeAt ( 0     )              ;
      start      ( c , V )              ;
    break                               ;
    default                             :
    return false                        ;
  }                                     ;
  return true                           ;
}

void N::FeatureExtractor::run(int Type,ThreadData * data)
{
  VarArgs V = data->Arguments ;
  switch   ( Type )           {
    case 3001                 :
      DigFeaturePoints (   )  ;
    break                     ;
    case 3002                 :
      CalculateFeature ( V )  ;
    break                     ;
  }                           ;
}

void N::FeatureExtractor::CalculateFeature(VarArgs V)
{
  SUID uuid = V[0].toULongLong()          ;
  if (!Images.contains(uuid)) return      ;
  PictureManager PM ( plan )              ;
  SurfLock . lock   ( )                   ;
  QImage * image = Images[uuid]           ;
  SurfLock . unlock ( )                   ;
  if (IsNull(image))                      {
    SurfLock . lock   (      )            ;
    Images   . take   ( uuid )            ;
    SurfLock . unlock (      )            ;
    return                                ;
  }                                       ;
  /////////////////////////////////////////
  FeaturePoints points                    ;
  double        Hessian = V[1].toDouble() ;
  int           Octaves = V[2].toInt   () ;
  int           Layers  = V[3].toInt   () ;
  double        distance = 0              ;
  double        angle    = 0              ;
  double        X        = 0              ;
  double        Y        = 0              ;
  double        NX       = 0              ;
  double        NY       = 0              ;
  double        ND       = 0              ;
  int           total    = 0              ;
  QSizeF        SF ( image->size() )      ;
  QRectF        Range                     ;
  /////////////////////////////////////////
  PM . SurfFeaturePoints                  (
    *image                                ,
    points                                ,
    Hessian                               ,
    Octaves                               ,
    Layers                              ) ;
  /////////////////////////////////////////
  QByteArray B                            ;
  QByteArray C                            ;
  total = points.count()                  ;
  if (total>0)                            {
    B        = points . toByteArray ( )   ;
    distance = points . distance    ( )   ;
    X        = points . x           ( )   ;
    Y        = points . y           ( )   ;
    Range    = points . Range       ( )   ;
    for (int i=0;i<points.count();i++)    {
      angle  += points[i]->angle          ;
    }                                     ;
    angle    /= total                     ;
    if (distance>0.00001)                 {
      distance  = ::sqrt(distance)        ;
    }                                     ;
    points   /= SF                        ;
    ND        = points . distance    ( )  ;
    NX        = points . x           ( )  ;
    NY        = points . y           ( )  ;
    if (B.size()>0)                       {
      C = XzCompress ( B )                ;
    }                                     ;
  }                                       ;
  /////////////////////////////////////////
  SurfLock . lock   ( )                   ;
  SurfTotals    [ uuid ] = total          ;
  SurfDistances [ uuid ] = distance       ;
  SurfAngles    [ uuid ] = angle          ;
  SurfPoints    [ uuid ] = C              ;
  SurfX         [ uuid ] = X              ;
  SurfY         [ uuid ] = Y              ;
  SurfNX        [ uuid ] = NX             ;
  SurfNY        [ uuid ] = NY             ;
  SurfND        [ uuid ] = ND             ;
  SurfRanges    [ uuid ] = Range          ;
  SurfLock . unlock ( )                   ;
}

void N::FeatureExtractor::FillIn(SqlConnection & SC,QByteArray & BP)
{
  SurfLock . lock   ( )                  ;
  QString Q                              ;
  UUIDs   U = SurfPoints.keys()          ;
  SUID    z                              ;
  SurfLock . unlock ( )                  ;
  foreach (z,U)                          {
    SurfLock . lock   ( )                ;
    QImage   * img = Images        [ z ] ;
    QByteArray B   = SurfPoints    [ z ] ;
    int        T   = SurfTotals    [ z ] ;
    double     D   = SurfDistances [ z ] ;
    double     A   = SurfAngles    [ z ] ;
    double     X   = SurfX         [ z ] ;
    double     Y   = SurfY         [ z ] ;
    double     NX  = SurfNX        [ z ] ;
    double     NY  = SurfNY        [ z ] ;
    double     ND  = SurfND        [ z ] ;
    QRectF     RE  = SurfRanges    [ z ] ;
    double     l   = RE . left     (   ) ;
    double     r   = RE . top      (   ) ;
    double     w   = RE . width    (   ) ;
    double     h   = RE . height   (   ) ;
    int        P   = Types::Picture      ;
    int        M   = 0                   ;
    SurfLock . unlock ( )                ;
    //////////////////////////////////////
    if (T>0)                             {
      plan -> ProgressText               (
        ProgressID                       ,
        tr("Writing %1").arg(z)        ) ;
      Q = SC.sql.Update                  (
            PlanTable(FeaturePoints)     ,
            SC.sql.Where                 (
              4                          ,
              "uuid"                     ,
              "type"                     ,
              "method"                   ,
              "paraments"              ) ,
            12                           ,
            "points"                     ,
            "distance"                   ,
            "x"                          ,
            "y"                          ,
            "nx"                         ,
            "ny"                         ,
            "nd"                         ,
            "angle"                      ,
            "l"                          ,
            "r"                          ,
            "w"                          ,
            "h"                        ) ;
      SC . Prepare ( Q                 ) ;
      SC . Bind    ( "uuid"      , z   ) ;
      SC . Bind    ( "type"      , P   ) ;
      SC . Bind    ( "method"    , M   ) ;
      SC . Bind    ( "points"    , T   ) ;
      SC . Bind    ( "distance"  , D   ) ;
      SC . Bind    ( "x"         , X   ) ;
      SC . Bind    ( "y"         , Y   ) ;
      SC . Bind    ( "nx"        , NX  ) ;
      SC . Bind    ( "ny"        , NY  ) ;
      SC . Bind    ( "nd"        , ND  ) ;
      SC . Bind    ( "angle"     , A   ) ;
      SC . Bind    ( "l"         , l   ) ;
      SC . Bind    ( "r"         , r   ) ;
      SC . Bind    ( "w"         , w   ) ;
      SC . Bind    ( "h"         , h   ) ;
      SC . Bind    ( "paraments" , BP  ) ;
      SC . Exec    (                   ) ;
      Q = SC.sql.InsertInto              (
            PlanTable(KeyPoints)         ,
            5                            ,
            "uuid"                       ,
            "type"                       ,
            "method"                     ,
            "paraments"                  ,
            "features"                 ) ;
      SC . Prepare ( Q                 ) ;
      SC . Bind    ( "uuid"      , z   ) ;
      SC . Bind    ( "type"      , P   ) ;
      SC . Bind    ( "method"    , M   ) ;
      SC . Bind    ( "paraments" , BP  ) ;
      SC . Bind    ( "features"  , B   ) ;
      SC . Exec    (                   ) ;
    }                                    ;
    //////////////////////////////////////
    SurfLock      . lock   (   )         ;
    SurfAngles    . take   ( z )         ;
    SurfDistances . take   ( z )         ;
    SurfTotals    . take   ( z )         ;
    SurfPoints    . take   ( z )         ;
    SurfX         . take   ( z )         ;
    SurfY         . take   ( z )         ;
    SurfNX        . take   ( z )         ;
    SurfNY        . take   ( z )         ;
    SurfND        . take   ( z )         ;
    SurfRanges    . take   ( z )         ;
    Images        . take   ( z )         ;
    SurfLock      . unlock (   )         ;
    if (NotNull(img)) delete img         ;
  }                                      ;
}

void N::FeatureExtractor::RunScan   (
       SqlConnection & SC           ,
       QByteArray    & BP           ,
       int             TotalThreads ,
       SUID            u            ,
       int             id           ,
       bool          & actual       ,
       double          Hessian      ,
       int             Octaves      ,
       int             Layers       ,
       int             Grid         ,
       bool            firstTime    )
{
  PictureManager PM ( plan )                       ;
  QString  Q                                       ;
  QImage * II = NULL                               ;
  QImage * TB = NULL                               ;
  int      TV = Types::Picture                     ;
  int      MV = 0                                  ;
  ProgressValue = id                               ;
  TB = PM . Thumb   ( SC , u )                     ;
  II = PM . Picture ( SC , u )                     ;
  if ( NotNull    ( TB ) &&  NotNull    ( II )    &&
      !TB->isNull (    ) && !II->isNull (    )    &&
       II->width()>Grid  &&  II->height()>Grid   ) {
    VarArgs V                                      ;
    plan -> ProgressText                           (
      ProgressID                                   ,
      tr("Scan feature %1:[%2 x %3]"               )
      .arg(u                                       )
      .arg(II->width ()                            )
      .arg(II->height())                         ) ;
    ////////////////////////////////////////////////
    SurfLock . lock   ( )                          ;
    Images [ u ] = II                              ;
    SurfLock . unlock ( )                          ;
    if (firstTime)                                 {
      Q = SC.sql.InsertInto                        (
            PlanTable(FeaturePoints)               ,
            4                                      ,
            "uuid"                                 ,
            "type"                                 ,
            "method"                               ,
            "paraments"                          ) ;
      SC . Prepare ( Q                           ) ;
      SC . Bind    ( "uuid"      , u             ) ;
      SC . Bind    ( "type"      , TV            ) ;
      SC . Bind    ( "method"    , MV            ) ;
      SC . Bind    ( "paraments" , BP            ) ;
      SC . Exec    (                             ) ;
    }                                              ;
    if (TotalThreads<=1)                           {
      V << u                                       ;
      V << Hessian                                 ;
      V << Octaves                                 ;
      V << Layers                                  ;
      CalculateFeature ( V )                       ;
      if (SurfPoints.count()>0) FillIn ( SC , BP ) ;
    } else                                         {
      V << 3002                                    ;
      V << u                                       ;
      V << Hessian                                 ;
      V << Octaves                                 ;
      V << Layers                                  ;
      Sequences << V                               ;
      actual = true                                ;
    }                                              ;
    ////////////////////////////////////////////////
  } else                                           {
    if (NotNull(II)) delete II                     ;
  }                                                ;
  if (NotNull(TB)) delete TB                       ;
}

bool N::FeatureExtractor::CleanTHs        (
       SqlConnection & SC           ,
       QByteArray    & BP           ,
       int             TotalThreads )
{
  if (TotalThreads>1)                                        {
    //////////////////////////////////////////////////////////
    if (SurfPoints.count()>0) FillIn ( SC , BP )             ;
    //////////////////////////////////////////////////////////
    if (Images.count()>=TotalThreads)                        {
      Time::msleep(50)                                       ;
      return true                                            ;
    }                                                        ;
  }                                                          ;
  if (TotalThreads>1 && AllThreads.count()>1000)             {
    //////////////////////////////////////////////////////////
    QList<ThreadData *> Dead                                 ;
    for (int i=0;i<(AllThreads.count()-500);i++)             {
      if (AllThreads[i]->Running==2)                         {
        Dead << AllThreads[i]                                ;
      }                                                      ;
    }                                                        ;
    //////////////////////////////////////////////////////////
    if (Dead.count()>0)                                      {
      for (int i=0;i<Dead.count();i++)                       {
        ThreadData * data = Dead[i]                          ;
        int           index                                  ;
        index = AllThreads.indexOf(data)                     ;
        if (index>=0)                                        {
          AllThreads . takeAt ( index )                      ;
          delete data                                        ;
        }                                                    ;
      }                                                      ;
    }                                                        ;
    //////////////////////////////////////////////////////////
  }                                                          ;
  return false                                               ;
}

void N::FeatureExtractor::DigFeaturePoints (void)
{
  PictureManager PM ( plan      )                                      ;
  SqlConnection  SC ( plan->sql )                                      ;
  if (SC.open("PictureCLI","DigFeaturePoints"))                        {
    QString       Q                                                    ;
    bool          done         = false                                 ;
    SUID          Last         = 0                                     ;
    int           total        = 0                                     ;
    double        Hessian      = 400.0                                 ;
    int           Octaves      = 3                                     ;
    int           Layers       = 4                                     ;
    int           TotalThreads = 12                                    ;
    int           doScan       = 0                                     ;
    int           Grid         = 16                                    ;
    int           Blocks       = 1000                                  ;
    bool          actual                                               ;
    UUIDs         Uuids                                                ;
    RMAPs         IDs                                                  ;
    SurfParaments SP                                                   ;
    QByteArray    BP                                                   ;
    ParamentQuery PQ ( Dynamic::Functionality , 1 , "SURF"        )    ;
    ////////////////////////////////////////////////////////////////////
    Octaves      = PQ . Value    (SC,PlanTable(Paraments),"Octaves"  ) ;
    Layers       = PQ . Value    (SC,PlanTable(Paraments),"Layers"   ) ;
    TotalThreads = PQ . Value    (SC,PlanTable(Paraments),"Threads"  ) ;
    doScan       = PQ . Value    (SC,PlanTable(Paraments),"Scan"     ) ;
    Grid         = PQ . Value    (SC,PlanTable(Paraments),"Grid"     ) ;
    Blocks       = PQ . Value    (SC,PlanTable(Paraments),"Blocks"   ) ;
    ////////////////////////////////////////////////////////////////////
    PQ.Variety   = 2                                                   ;
    Hessian      = PQ . Floating (SC,PlanTable(Paraments),"Hessian"  ) ;
    ////////////////////////////////////////////////////////////////////
    SP.Hessian = Hessian                                               ;
    SP.Octaves = Octaves                                               ;
    SP.Layers  = Layers                                                ;
    BP.append((const char *)&SP,sizeof(SurfParaments))                 ;
    ////////////////////////////////////////////////////////////////////
    Q = SC.sql.SelectFrom("count(*)",PlanTable(ThumbUuid))             ;
    if (SC.Fetch(Q)) total = SC.Int(0)                                 ;
    if (total>0)                                                       {
      int TV = Types::Picture                                          ;
      int MV = 0                                                       ;
      Q = SC.sql.SelectFrom                                            (
            "uuid"                                                     ,
            PlanTable(FeaturePoints)                                   ,
            SC.sql.Where(3,"type","method","paraments")                ,
            SC.OrderByDesc("id")                                       ,
            SC.sql.Limit(0,1)                                        ) ;
      SC . Prepare ( Q                )                                ;
      SC . Bind    ( "type"      , TV )                                ;
      SC . Bind    ( "method"    , MV )                                ;
      SC . Bind    ( "paraments" , BP )                                ;
      if (SC.Exec() && SC.Next()) Last = SC.Uuid(0)                    ;
      if (doScan>0)                                                    {
        int PTs = 0                                                    ;
        Q = SC.sql.SelectFrom                                          (
              "count(*)"                                               ,
              PlanTable(FeaturePoints)                                 ,
              SC.sql.Where(4,"type","method","paraments","points")   ) ;
        SC . Prepare ( Q                 )                             ;
        SC . Bind    ( "type"      , TV  )                             ;
        SC . Bind    ( "method"    , MV  )                             ;
        SC . Bind    ( "paraments" , BP  )                             ;
        SC . Bind    ( "points"    , PTs )                             ;
        if (SC.Exec() && SC.Next()) PTs = SC.Int(0)                    ;
        if (PTs>0)                                                     {
          ProgressGo    = true                                         ;
          ProgressValue = 0                                            ;
          plan -> Start ( ProgressID,&ProgressValue,&ProgressGo )      ;
          done = false                                                 ;
          while (!done)                                                {
            int XZV = 0                                                ;
            PTs = 0                                                    ;
            Q = SC.sql.SelectFrom                                      (
                  "count(*)"                                           ,
                  PlanTable(FeaturePoints)                             ,
                  SC.sql.Where                                         (
                    4                                                  ,
                    "type"                                             ,
                    "method"                                           ,
                    "paraments"                                        ,
                    "points"                                       ) ) ;
            SC . Prepare ( Q                 )                         ;
            SC . Bind    ( "type"      , TV  )                         ;
            SC . Bind    ( "method"    , MV  )                         ;
            SC . Bind    ( "paraments" , BP  )                         ;
            SC . Bind    ( "points"    , XZV )                         ;
            if (SC.Exec() && SC.Next()) PTs = SC.Int(0)                ;
            plan->setRange(ProgressID,0,PTs)                           ;
            ////////////////////////////////////////////////////////////
            Uuids . clear ( )                                          ;
            IDs   . clear ( )                                          ;
            Q = SC.sql.SelectFrom                                      (
                  "uuid"                                               ,
                  PlanTable(FeaturePoints)                             ,
                  SC.sql.Where(4,"type","method","paraments","points") ,
                  SC.OrderByAsc("id")                                ) ;
            SC . Prepare ( Q                 )                         ;
            SC . Bind    ( "type"      , TV  )                         ;
            SC . Bind    ( "method"    , MV  )                         ;
            SC . Bind    ( "paraments" , BP  )                         ;
            SC . Bind    ( "points"    , XZV )                         ;
            if (SC.Exec()) while (SC.Next()) Uuids << SC.Uuid(0)       ;
            ////////////////////////////////////////////////////////////
            if (Uuids.count()==0) done = true; else                    {
              for (int z=0;ProgressGo && z<Uuids.count();)             {
                if (CleanTHs(SC,BP,TotalThreads)) continue             ;
                actual = false                                         ;
                while (   ProgressGo                                  &&
                        ( Images.count() < TotalThreads  )            &&
                        ( z              < Uuids.count() )           ) {
                  SUID u  = Uuids [ z ]                                ;
                  int  id = z                                          ;
                  RunScan                                              (
                    SC                                                 ,
                    BP                                                 ,
                    TotalThreads                                       ,
                    u                                                  ,
                    id                                                 ,
                    actual                                             ,
                    Hessian                                            ,
                    Octaves                                            ,
                    Layers                                             ,
                    Grid                                               ,
                    false                                            ) ;
                  z++                                                  ;
                }                                                      ;
                if ( actual ) emit TriggerCommand ( )                  ;
              }                                                        ;
            }                                                          ;
            ////////////////////////////////////////////////////////////
            done = true                                                ;
          }                                                            ;
        }                                                              ;
        if (ProgressGo) done = false                                   ;
      }                                                                ;
      if (Last<=0)                                                     {
        ProgressValue = 0                                              ;
      } else                                                           {
        Q = SC.sql.SelectFrom                                          (
              "id"                                                     ,
              PlanTable(PictureUuid)                                   ,
              SC.WhereUuid(Last)                                     ) ;
        if (SC.Fetch(Q)) ProgressValue = SC.Int(0)                     ;
      }                                                                ;
      ProgressGo    = true                                             ;
      plan -> Start ( ProgressID , &ProgressValue , &ProgressGo )      ;
      while (!done)                                                    {
        ////////////////////////////////////////////////////////////////
        Q = SC.sql.SelectFrom                                          (
              "id",PlanTable(PictureUuid)                              ,
              SC.OrderByDesc("id")                                     ,
              SC.sql.Limit(0,1)                                      ) ;
        if (SC.Fetch(Q)) total = SC . Int ( 0 )                        ;
        plan->setRange(ProgressID,0,total)                             ;
        ////////////////////////////////////////////////////////////////
        plan -> ProgressText                                           (
          ProgressID                                                   ,
          tr ( "Fetch %1 to %2"                                        )
            . arg ( ProgressValue                                      )
            . arg ( ProgressValue + Blocks                         ) ) ;
        ////////////////////////////////////////////////////////////////
        Uuids . clear ( )                                              ;
        IDs   . clear ( )                                              ;
        Q = SC.sql.SelectFrom                                          (
              "id,uuid"                                                ,
              PlanTable(PictureUuid)                                   ,
              QString("where id > %1").arg(ProgressValue)              ,
              SC.sql.Limit(Blocks)                                   ) ;
        SqlLoopNow ( SC , Q )                                          ;
          int  ID   = SC . Int  ( 0 )                                  ;
          SUID suid = SC . Uuid ( 1 )                                  ;
          Uuids        << suid                                         ;
          IDs [ suid ]  = ID                                           ;
        SqlLoopErr ( SC , Q )                                          ;
        SqlLoopEnd ( SC , Q )                                          ;
        ////////////////////////////////////////////////////////////////
        if (Uuids.count()==0) done = true; else                        {
          for (int z=0;ProgressGo && z<Uuids.count();)                 {
            if (CleanTHs(SC,BP,TotalThreads)) continue                 ;
            actual = false                                             ;
            while (   ProgressGo                                      &&
                    ( Images.count() < TotalThreads  )                &&
                    ( z              < Uuids.count() )               ) {
              SUID u  = Uuids [ z ]                                    ;
              int  id = IDs   [ u ]                                    ;
              RunScan                                                  (
                SC                                                     ,
                BP                                                     ,
                TotalThreads                                           ,
                u                                                      ,
                id                                                     ,
                actual                                                 ,
                Hessian                                                ,
                Octaves                                                ,
                Layers                                                 ,
                Grid                                                   ,
                true                                                 ) ;
              z++                                                      ;
            }                                                          ;
            if ( actual ) emit TriggerCommand ( )                      ;
          }                                                            ;
        }                                                              ;
        ////////////////////////////////////////////////////////////////
        if (!ProgressGo) done = true                                   ;
      }                                                                ;
      //////////////////////////////////////////////////////////////////
      if (TotalThreads>1)                                              {
        while (Images.count()>0)                                       {
          if (SurfPoints.count()<=0)                                   {
            Time::msleep(50)                                           ;
            continue                                                   ;
          }                                                            ;
          FillIn ( SC , BP )                                           ;
        }                                                              ;
      }                                                                ;
    }                                                                  ;
    ////////////////////////////////////////////////////////////////////
    SC . close ( )                                                     ;
  }                                                                    ;
  SC.remove()                                                          ;
  plan -> Finish ( ProgressID                        )                 ;
  plan -> Talk   ( tr("Feature extraction finished") )                 ;
  Alert             ( Done )                                           ;
  emit SafeComplete (      )                                           ;
}

bool N::FeatureExtractor::startup(void)
{
  ProgressID    = plan->Progress            (
                    tr("Feature scanner")   ,
                    tr("%v/%m pictures" ) ) ;
  ProgressValue = 0                         ;
  ProgressGo    = false                     ;
  start            ( 3001 )                 ;
  return true                               ;
}
