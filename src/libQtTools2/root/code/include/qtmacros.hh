/*********************************************************************/
/*                                                                   */
/* FILE         qtmacros.hh                                          */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Some macros for automatically generating qt func-    */
/*              tions for handling line edits, check boxes, etc.     */
/*                                                                   */
/* BEGIN        Tue Jan 28 2003                                      */
/* LAST CHANGE  Tue Jan 28 2003                                      */
/*                                                                   */
/*********************************************************************/

#ifndef QTMACROS_HH
#define QTMACROS_HH

// Assumptions: 
//   - cname constains text a la 'MyWidget::slot'
//   - all 'set'       functions are called slotSet...()
//   - all 'update'    functions are called slotUpdate...()
//   - all checkbox    functions are called slotSet...OnOff()
//   - all radiobutton functions are called slotSelect...()

/*===================================================================*/
/*                     Connection Declarations                       */
/*===================================================================*/

/*****************/
/*   Lineedits   */
/*****************/

#define QT_CONNECT_LINEEDIT( qname, vname)                       \
connect( qname, SIGNAL(textChanged(const QString&)),             \
         this, SLOT(slotSet ## vname (const QString&)) );        \
connect( qname, SIGNAL(returnPressed()),                         \
         this, SLOT(slotUpdate ## vname ()) );                   \
connect( this, SIGNAL(sig ## vname ## Changed(const QString&)),  \
         qname, SLOT(setText(const QString&)) )                  \


/******************/
/*   Checkboxes   */
/******************/

#define QT_CONNECT_CHECKBOX( qname, vname)                     \
connect( qname, SIGNAL(stateChanged(int)),                     \
         this, SLOT(slotSet ## vname ## OnOff(int)) )          \


/********************/
/*   Radiobuttons   */
/********************/

#define QT_CONNECT_RADIOBUTTON( qname, vname)                     \
connect( qname, SIGNAL(clicked(int)),                             \
         this, SLOT( slotSelect ## vname (int)) )                 \


/*===================================================================*/
/*                     Function Implementations                      */
/*===================================================================*/

/*****************/
/*   Lineedits   */
/*****************/

#define QT_IMPLEMENT_LINEEDIT_INT( cname, vname, var ) \
                                                     \
void cname ## Set ## vname ( const QString &text )   \
  /* set the variable value to the input text */     \
{                                                    \
  var = atoi( text );                                \
}                                                    \
                                                     \
void cname ## Update ## vname ()                     \
  /* update the variable value in the edit field */  \
{                                                    \
  QString newtext;                                   \
  newtext = newtext.setNum( var );                   \
  emit sig ## vname ## Changed( newtext );           \
}                                                    


#define QT_IMPLEMENT_LINEEDIT_FLOAT( cname, vname, var, num )  \
                                                     \
void cname ## Set ## vname ( const QString &text )   \
  /* set the variable value to the input text */     \
{                                                    \
  var = atof( text );                                \
}                                                    \
                                                     \
void cname ## Update ## vname ()                     \
  /* update the variable value in the edit field */  \
{                                                    \
  QString newtext;                                   \
  newtext = newtext.setNum( var , 'f', num );        \
  emit sig ## vname ## Changed( newtext );           \
}                                                    

#define QT_IMPLEMENT_LINEEDIT_STRING( cname, vname, var )  \
                                                     \
void cname ## Set ## vname ( const QString &text )   \
  /* set the variable value to the input text */     \
{                                                    \
  var = text.latin1();                               \
}                                                    \
                                                     \
void cname ## Update ## vname ()                     \
  /* update the variable value in the edit field */  \
{                                                    \
  QString newtext;                                   \
  newtext = var;                                     \
  emit sig ## vname ## Changed( newtext );           \
}                                                    


/******************/
/*   Checkboxes   */
/******************/

#define QT_IMPLEMENT_CHECKBOX( cname, vname, var )     \
                                                     \
void cname ## Set ## vname ## OnOff( int state )     \
  /* set the variable value to the checkbox state */ \
{                                                    \
  if ( state == 0 ) var = false;                     \
  else var = true;                                   \
}                                          


/********************/
/*   Radiobuttons   */
/********************/

#define QT_IMPLEMENT_RADIOBUTTON( cname, vname, var )  \
                                                     \
void cname ## Select ## vname ( int id )             \
  /* set the radiobutton value */                    \
{                                                    \
  var = id;                                          \
}                                                    


#endif
