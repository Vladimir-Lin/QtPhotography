#include <qtphotography.h>

static unsigned char BitMasks[8] = {
  0x01                             ,
  0x02                             ,
  0x04                             ,
  0x08                             ,
  0x10                             ,
  0x20                             ,
  0x40                             ,
  0x80                           } ;

static int obtainInt(unsigned char * bits)
{
  int v = 0              ;
  for (int i=0;i<32;i++) {
    unsigned char z      ;
    int m = i % 3        ;
    int x = i / 3        ;
    x *= 4               ;
    x += m               ;
    z  = bits[x]         ;
    z &= 0x01            ;
    if (z>0)             {
      int X = z          ;
      if (i>0) X <<= i   ;
      v |= X             ;
    }                    ;
  }                      ;
  return v               ;
}

static int obtainUInt(unsigned char * bits)
{
  unsigned int v = 0     ;
  for (int i=0;i<32;i++) {
    unsigned char z      ;
    int m = i % 3        ;
    int x = i / 3        ;
    x  *= 4              ;
    x  += m              ;
    z   = bits[x]        ;
    z  &= 0x01           ;
    if (z>0)             {
      unsigned int X = z ;
      if (i>0) X <<= i   ;
      v |= X             ;
    }                    ;
  }                      ;
  return v               ;
}

static void putInt(unsigned char * bits,int v)
{
  for (int i=0;i<32;i++) {
    unsigned char z      ;
    int m     = i % 3    ;
    int x     = i / 3    ;
    x        *= 4        ;
    x        += m        ;
    z         = v & 1    ;
    v       >>= 1        ;
    bits[x]  &= 0xFE     ;
    bits[x]  |= z        ;
  }                      ;
}

static void putUInt(unsigned char * bits,unsigned int v)
{
  for (int i=0;i<32;i++) {
    unsigned char z      ;
    int m     = i % 3    ;
    int x     = i / 3    ;
    x        *= 4        ;
    x        += m        ;
    z         = v & 1    ;
    v       >>= 1        ;
    bits[x]  &= 0xFE     ;
    bits[x]  |= z        ;
  }                      ;
}

static void putData(unsigned char * bits,unsigned char * data,int length)
{
  for (int i=0;i<length;i++)                       {
    for (int j=0;j<8;j++)                          {
      int x    = ( i * 8 ) + j                     ;
      int m    = x % 3                             ;
      int f    = x / 3                             ;
      int v    = ( data[i] & BitMasks[j] ) ? 1 : 0 ;
      f       *= 4                                 ;
      f       += m                                 ;
      bits[f] &= 0xFE                              ;
      bits[f] |= (unsigned char)v                  ;
    }                                              ;
  }                                                ;
}

static void obtainData(unsigned char * bits,unsigned char * data,int length)
{
  for (int i=0;i<length;i++)      {
    unsigned char d = 0           ;
    for (int j=0;j<8;j++)         {
      int x    = ( i * 8 ) + j    ;
      int m    = x % 3            ;
      int f    = x / 3            ;
      int v    = 0                ;
      f       *= 4                ;
      f       += m                ;
      v        = bits[f] & 1      ;
      if (v>0)                    {
        if (j>0) v <<= j          ;
        d     |= (unsigned char)v ;
      }                           ;
    }                             ;
    data[i] = d                   ;
  }                               ;
}

/****************************************************************************\
 *                                                                          *
 *                            Embeded Image Format                          *
 *                                                                          *
 * 1. First      8 pixels  : Blank (0-8)                                    *
 * 2. Lastest    8 pixels  : Blank (0-8)                                    *
 * 3. First      8-19      : 12 Pixels , size of data                       *
 * 4. Lastest    8-19      : 12 Pixels , checksum of data                   *
 * 5. Signature 20-35      : Foxman ( 6 bytes * 8 = 48 Bits = 16 pixels )   *
 * 6. Equation parament    : Lastest 20-31                                  *
 * 7. Mapping parament     : Lastest 32-43                                  *
 *                                                                          *
\****************************************************************************/

bool N::Images::Embedded(QImage & input,QImage & output,QByteArray & body)
{
  output = input.convertToFormat(QImage::Format_ARGB32) ;
  int             size = body   . size   ( )            ;
  int             w    = output . width  ( )            ;
  int             h    = output . height ( )            ;
  int             t    = w      * h                     ;
  int             z    = size   * 8                     ;
  unsigned int    chk  = ADLER32 ( body )               ;
  unsigned char * b    = (unsigned char *)output.bits() ;
  unsigned char * s    = (unsigned char *)body  .data() ;
  unsigned char * l    = (unsigned char *)output.bits() ;
  const char    * FOX  = "Foxman"                       ;
  l += t * 4                                            ;
  t -= 80                                               ;
  t *= 3                                                ;
  if (z>t) return false                                 ;
  b += 8 * 4                                            ;
  putInt  ( b , size     )                              ;
  b += 12 * 4                                           ;
  putData ( b , (unsigned char *)FOX , 6  )             ;
  b += 16 * 4                                           ;
  l -=  8 * 4                                           ;
  l -= 12 * 4                                           ;
  putUInt ( l , chk      )                              ;
  l -= 12 * 4                                           ;
  putInt  ( l , 0        )                              ;
  l -= 12 * 4                                           ;
  putInt  ( l , 0        )                              ;
  putData ( b , s , size )                              ;
  return true                                           ;
}

bool N::Images::Extract(QImage & input,QByteArray & body)
{
  int             w    = input . width  ( )            ;
  int             h    = input . height ( )            ;
  int             t    = w     * h                     ;
  unsigned char * b    = (unsigned char *)input.bits() ;
  unsigned char * l    = (unsigned char *)input.bits() ;
  int             size = 0                             ;
  unsigned int    chk  = 0                             ;
  unsigned int    vry  = 0                             ;
  int             equ  = 0                             ;
  int             map  = 0                             ;
  char            FOX[16]                              ;
  //////////////////////////////////////////////////////
  body . clear ( )                                     ;
  if (t<80) return false                               ;
  //////////////////////////////////////////////////////
  l +=  t * 4                                          ;
  //////////////////////////////////////////////////////
  b +=  8 * 4                                          ;
  size = obtainInt ( b )                               ;
  if (size<=0) return false                            ;
  b += 12 * 4                                          ;
  //////////////////////////////////////////////////////
  l -=  8 * 4                                          ;
  l -= 12 * 4                                          ;
  chk = obtainUInt ( l )                               ;
  l -= 12 * 4                                          ;
  equ = obtainInt  ( l )                               ;
  l -= 12 * 4                                          ;
  map = obtainInt  ( l )                               ;
  //////////////////////////////////////////////////////
  memset     ( FOX , 0   , 16 )                        ;
  obtainData ( b   , (unsigned char *)FOX ,  6 )       ;
  if (strcmp(FOX,"Foxman")!=0) return false            ;
  b += 16 * 4                                          ;
  //////////////////////////////////////////////////////
  body . resize ( size )                               ;
  unsigned char * s    = (unsigned char *)body .data() ;
  obtainData ( b , s , size )                          ;
  vry = ADLER32 ( body )                               ;
  //////////////////////////////////////////////////////
  if (vry!=chk)                                        {
    body .clear ( )                                    ;
    return false                                       ;
  }                                                    ;
  //////////////////////////////////////////////////////
  return true                                          ;
}

///////////////////////////////////////////////////////////////////////////////

typedef struct    {
  int  type       ;
  int  pack       ;
  SUID signature  ;
  char buffer[48] ;
} SteganoSource   ;

typedef struct    {
  int  type       ;
  int  method     ;
  SUID signature  ;
  int  types [ 4] ;
  int  sizes [ 4] ;
  char buffer[16] ;
} SteganoKey      ;

const char * keylists = "0123456789"
                        "abcdefghijklmnopqrstuvwxyz"
                        "ABCDEFGHIJKLMNOPQRSTUVWXZY"
                        "#@" ;

QString RandomKey(int length)
{
  QString L                  ;
  for (int i=0;i<length;i++) {
    int r = rand() % 62      ;
    L . append (keylists[r]) ;
  }                          ;
  return  L                  ;
}

int N::Images::DecryptPair(QByteArray & Body,SUID & signature)
{
  int t = -1                                         ;
  if (Body.size()<64) return t                       ;
  SteganoSource * ss = (SteganoSource * )Body.data() ;
  t         = ss->type                               ;
  signature = ss->signature                          ;
  return t                                           ;
}

bool N::Images::EncryptPair   (
       SUID         signature ,
       QByteArray & Source    ,
       QByteArray & Encrypted ,
       QByteArray & Keys      )
{
  QByteArray    SrcData     = Source                             ;
  QByteArray    EncryptData                                      ;
  QByteArray    KeyData                                          ;
  CUIDs         IDs                                              ;
  int           total       = 0                                  ;
  SteganoSource SRC                                              ;
  SteganoKey    KEY                                              ;
  ////////////////////////////////////////////////////////////////
  IDs = N::Encryptors()                                          ;
  ////////////////////////////////////////////////////////////////
  memset ( &SRC , 0 , sizeof(SteganoSource) )                    ;
  memset ( &KEY , 0 , sizeof(SteganoKey   ) )                    ;
  KeyData         . clear ( )                                    ;
  ////////////////////////////////////////////////////////////////
  SRC . type      = 1                                            ;
  SRC . pack      = 0                                            ;
  SRC . signature = signature                                    ;
  ////////////////////////////////////////////////////////////////
  KEY . type      = 0                                            ;
  KEY . method    = 0                                            ;
  KEY . signature = signature                                    ;
  ////////////////////////////////////////////////////////////////
  while (total<4)                                                {
    int            id   = ( rand() % IDs.count() )               ;
    int            type = IDs[id]                                ;
    N::Encryptor * e    = N::encryptor (type)                    ;
    if (NotNull(e))                                              {
      if (e->supports(N::Cryptography::Cipher    )              ||
          e->supports(N::Cryptography::Asymmetric)             ) {
        QStringList M  = e -> Methods ( )                        ;
        CUIDs       B  = e -> Bits    ( )                        ;
        QString     RK = RandomKey(16)                           ;
        QString     m  = M [ rand() % M.count() ]                ;
        int         b  = B [ rand() % B.count() ]                ;
        EncryptData   . clear ( )                                ;
        e->Arguments << b                                        ;
        e->Arguments << m                                        ;
        e->Arguments << RK                                       ;
        if ( e -> encrypt ( SrcData , EncryptData )             &&
             e -> Key . size ( ) > 0                            &&
             EncryptData . size ( ) > 0 )                        {
          KEY     . types [ total ] = type                       ;
          KEY     . sizes [ total ] = e->Key.size()              ;
          KeyData . append ( e->Key )                            ;
          SrcData = EncryptData                                  ;
          total ++                                               ;
        }                                                        ;
      }                                                          ;
      delete e                                                   ;
    }                                                            ;
  }                                                              ;
  ////////////////////////////////////////////////////////////////
  if (KeyData.size()<=0) return false                            ;
  if (SrcData.size()<=0) return false                            ;
  ////////////////////////////////////////////////////////////////
  Encrypted . clear  (                                        )  ;
  Encrypted . append ((const char *)&SRC,sizeof(SteganoSource))  ;
  Encrypted . append (SrcData                                 )  ;
  ////////////////////////////////////////////////////////////////
  Keys      . clear  (                                        )  ;
  Keys      . append ((const char *)&KEY,sizeof(SteganoKey   ))  ;
  Keys      . append (KeyData                                 )  ;
  ////////////////////////////////////////////////////////////////
  return true                                                    ;
}

unsigned char XzSignature [] = { 0xFD , 0x37 , 0x7A , 0x58 , 0x5A , 0x00 } ;

bool N::Images::DecryptBody   (
       SUID         signature ,
       QByteArray & Source    ,
       QByteArray & Encrypted ,
       QByteArray & Keys      )
{
  Source . clear ( )                                         ;
  ////////////////////////////////////////////////////////////
  SteganoSource * ss = (SteganoSource *)Encrypted . data ( ) ;
  SteganoKey    * sk = (SteganoKey    *)Keys      . data ( ) ;
  ////////////////////////////////////////////////////////////
  if ( signature   != ss -> signature ) return false         ;
  if ( signature   != sk -> signature ) return false         ;
  if ( ss ->type   != 1               ) return false         ;
  if ( ss ->pack   != 0               ) return false         ;
  if ( sk ->type   != 0               ) return false         ;
  if ( sk ->method != 0               ) return false         ;
  ////////////////////////////////////////////////////////////
  QByteArray B = Encrypted                                   ;
  B . remove ( 0 , sizeof(SteganoSource) )                   ;
  QMap<int,int       > TYPEs                                 ;
  QMap<int,QByteArray> KEYs                                  ;
  int                  index = sizeof(SteganoKey)            ;
  ////////////////////////////////////////////////////////////
  for (int i=0;i<4;i++)                                      {
    QByteArray K                                             ;
    int        s     = sk->sizes[i]                          ;
    TYPEs [ 3 - i ]  = sk->types[i]                          ;
    K                = Keys . mid ( index , s )              ;
    KEYs  [ 3 - i ]  = K                                     ;
    index           += s                                     ;
  }                                                          ;
  ////////////////////////////////////////////////////////////
  bool correct = true                                        ;
  for (int i=0;correct && i<4;i++)                           {
    int            t = TYPEs        [ i ]                    ;
    QByteArray     K = KEYs         [ i ]                    ;
    N::Decryptor * d = N::decryptor ( t )                    ;
    if (NotNull(d))                                          {
      QByteArray Z                                           ;
      d -> Key = K                                           ;
      if ( d -> decrypt ( B , Z ) )                          {
        if (Z.size()<=0) correct = false ; else B = Z        ;
      } else correct = false                                 ;
      delete d                                               ;
    }                                                        ;
  }                                                          ;
  if (!correct) B . clear ( )                                ;
  ////////////////////////////////////////////////////////////
  if (B.size()>6)                                            {
    unsigned char * xz = (unsigned char *)B.data()           ;
    if (memcmp(xz,XzSignature,6)==0) Source = B              ;
  }                                                          ;
  return ( Source . size ( ) > 0 )                           ;
}
