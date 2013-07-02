
#ifndef CANDIDATE_H
#define CANDIDATE_H

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <set>
#include <iostream>

#include <qstring.h>
#include <qrect.h>
#include <imgdescrlist.hh>

class Candidate
{
public:
  Candidate( QRect p, double d, QString s, int i )
  {
    //position=p;
    //distance=d;
    //name=s;
    //index=i;
    m_qrPos       = p;
    m_dDist       = d;
    m_sName       = s.latin1();
    m_nIdx        = i;
  }
  
  Candidate( QRect p, float d, int templId, int i )
  {
    m_qrPos       = p;
    m_dDist       = d;
    m_nTemplateId = templId;
    m_nIdx        = i;
  }

  Candidate()
  {
    //position=QRect(0,0,0,0);
    //distance=1000;
    m_qrPos       = QRect(0,0,0,0);
    m_dDist       = 9999.9;
    m_nIdx        = -1;
    m_nTemplateId = -1;
    m_sName       = "";
  }
  
  virtual ~Candidate() {}

  Rect getRect() const 
  {
    return Rect( m_qrPos.x(), m_qrPos.y(), 
                 m_qrPos.x()+ m_qrPos.width(), 
                 m_qrPos.y()+ m_qrPos.height(), m_dDist, m_nTemplateId );
  }
  
  inline int    getIdx()        const { return m_nIdx; }
  inline float  getDist()       const { return m_dDist; }
  inline QRect  getPos()        const { return m_qrPos; }
  inline int    getTemplateId() const { return m_nTemplateId; }
  inline string getName()       const { return m_sName; }

  inline int&    getIdx()             { return m_nIdx; }
  inline float&  getDist()            { return m_dDist; }
  inline QRect&  getPos()             { return m_qrPos; }
  inline int&    getTemplateId()      { return m_nTemplateId; }
  inline string& getName()            { return m_sName; }

  virtual void print() const
  {
    cout << "chamf=" << m_dDist 
         << ", pos=(" << m_qrPos.x() << "," << m_qrPos.y() 
         << "), templateId=" << m_nTemplateId << endl;
  }

  friend struct compareCandidates;

protected:
  //unsigned int index;
  //String      name;
  //QRect        position;
  //double       distance;

  int     m_nIdx;
  int     m_nTemplateId;
  string  m_sName;
  QRect   m_qrPos;
  float   m_dDist;
};


class CombiCandidate : public Candidate
{
public:
  CombiCandidate( QRect p, float d, int templId, int i )
    : Candidate( p, d, templId, i )
  {
    m_dScale      = 1.0;
    m_dBhScore    = 0.0;
    m_dCombScore  = 0.0;
  }

  CombiCandidate( QRect p, float s, float d, int templId, int i )
    : Candidate( p, d, templId, i )
  {
    m_dScale      = s;
    m_dBhScore    = 0.0;
    m_dCombScore  = 0.0;
  }

  CombiCandidate( QRect p, float s, float d, float bh, int templId, int i )
    : Candidate( p, d, templId, i )
  {
    m_dScale      = s;
    m_dBhScore    = bh;
    m_dCombScore  = 0.0;
  }

  CombiCandidate( QRect p, float s, float d, float bh, float cb, 
                  int templId, int i )
    : Candidate( p, d, templId, i )
  {
    m_dScale      = s;
    m_dBhScore    = bh;
    m_dCombScore  = cb;
  }

  CombiCandidate()
    : Candidate()
  {
    m_dScale      = 1.0;
    m_dBhScore    = 0.0;
    m_dCombScore  = 0.0;
  }
  
  void print() const
  {
    cout << "chamf=" << m_dDist << ", bh=" << m_dBhScore 
         << ", comb=" << m_dCombScore
         << ", pos=(" << m_qrPos.x() << "," << m_qrPos.y() 
         << "," << m_dScale << "), templateId=" << m_nTemplateId << endl;
  }

  inline float getScale()     const { return m_dScale; }
  inline float getBhScore()   const { return m_dBhScore; }
  inline float getCombScore() const { return m_dCombScore; }

  inline float& getScale()          { return m_dScale; }
  inline float& getBhScore()        { return m_dBhScore; }
  inline float& getCombScore()      { return m_dCombScore; }

  friend struct compareCandidates;

protected:
  float   m_dScale;
  float   m_dBhScore;
  float   m_dCombScore;
};


struct compareCandidates
{
public:
  bool operator()( const Candidate& c1, const Candidate& c2 )
  {
    if (c1.m_dDist!=c2.m_dDist)
      return c1.m_dDist < c2.m_dDist;
    if (c1.m_qrPos!=c2.m_qrPos)
      if (c1.m_qrPos.x()!=c2.m_qrPos.x())
        return c1.m_qrPos.x() < c2.m_qrPos.x();
      else
        return c1.m_qrPos.y() < c2.m_qrPos.y();
    if (c1.m_nTemplateId!=c2.m_nTemplateId)
      return c1.m_nTemplateId < c2.m_nTemplateId;
    if (c1.m_sName!=c2.m_sName)
      return c1.m_sName < c2.m_sName;
    return false;
  }
  
  bool operator()( const CombiCandidate& c1, const CombiCandidate& c2 )
  {
    if (c1.m_dCombScore!=c2.m_dCombScore)
      return c1.m_dCombScore > c2.m_dCombScore;
    if (c1.m_dDist!=c2.m_dDist)
      return c1.m_dDist < c2.m_dDist;
    if (c1.m_dScale!=c2.m_dScale)
      return c1.m_dScale < c2.m_dScale;
    if (c1.m_qrPos!=c2.m_qrPos)
      if (c1.m_qrPos.x()!=c2.m_qrPos.x())
        return c1.m_qrPos.x() < c2.m_qrPos.x();
      else
        return c1.m_qrPos.y() < c2.m_qrPos.y();
    if (c1.m_nTemplateId!=c2.m_nTemplateId)
      return c1.m_nTemplateId < c2.m_nTemplateId;
    if (c1.m_dBhScore!=c2.m_dBhScore)
      return c1.m_dBhScore > c2.m_dBhScore;
    return false;
  }

};

struct compareQPoints
{
public:
  bool operator()(const QPoint& p1, const QPoint& p2)
  {
    if (p1.x()!=p2.x())
      return p1.x()< p2.x();
    if (p1.y()!=p2.y())
      return p1.y()< p2.y();
    return false;
  }
};


typedef set<Candidate, compareCandidates>      CandidateSet;
typedef set<CombiCandidate, compareCandidates> CombiCandidateSet;

#endif

