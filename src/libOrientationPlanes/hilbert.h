/*
 * Copyright (c) 2000-2001. The Regents of the University of California
 * (Regents). All Rights Reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for educational, research, and not-for-profit
 * purposes, without fee and without a signed licensing agreement, is
 * hereby granted, provided that the above copyright notice, this
 * paragraph and the following two paragraphs appear in all copies,
 * modifications, and distributions.  Contact The Office of Technology
 * Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
 * CA 94720-1620, (510) 643-7201, for commercial licensing
 * opportunities.  The software uses the Normalized Cuts approach,
 * ("Normalized Cuts and Image Segmentation", Jianbo Shi and Jitendra
 * Malik, IEEE Conf. Computer Vision and Pattern Recognition, June
 * 1997, Puerto Rico and (longer version) IEEE Trans. PAMI, Sept
 * 2000).  The inter-pixel similarity measure W_ij is based on texture
 * and/or contour analysis, ("Contour and image analysis for
 * segmentation", Jitendra Malik, Serge Belongie, Thomas Leung and
 * Jianbo Shi, in "Perceptual Organization for Artificial Vision
 * Systems", K.L. Boyer and S. Sarkar, editors. Kluwer Academic
 * Publishers, 2000, pp. 139-172, and IJCV (forthcoming)).  This
 * software was created by Doron Tal based on previous code and
 * algorithms described in the above-mentioned papers.  David Martin
 * optimized some parts of this software, as documented in the source
 * code.  The software makes use of the fortran sparse matrix
 * eigensolver snlasod.f, (from the LASO package, written by David
 * Scott, retrieved from NETLIB on May 10, 2000).  The f2c libraries
 * are included in this package and are copyrighted by At&T, Lucent
 * Technologies and Bellcore (please see the license files in
 * ./f2c/libF77/Notice and ./f2c/libI77/Notice).  This software uses
 * mixed-radix Fast Fourier Transform code (from package GO, retrieved
 * from NETLIB on July 5, 1995).  This software also uses the package
 * 'triangle', created and copyrighted by Jonathan R. Shewchuck for
 * performing Delaunay triangulation - see the file
 * './triangle/readme' for copyright and license information related
 * to triangle.c.  For copyright and license information related to
 * the Independent Jpeg Group's JPEG library, also included in this
 * software, consult the file ./jpeg/README.  For bug reports and
 * technical information contact doron@cs.berkeley.edu.
 * 
 * IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 * SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
 * DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS
 * IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
 * UPDATES, ENHANCEMENTS, OR MODIFICATIONS.  
 */
/////////////////////////////////////////////////////////////////////////////
// hilbert.h - hilbert transform
// Author: Doron Tal (this code is based on the matlab (v5.3) 'hilbert.m')
// Date created: March, 2000

#ifndef _HILBERT_H
#define _HILBERT_H

namespace DTLib {

    // In-place hilbert transform on aInputVector.
    void Hilbert(float *aInVector, const int Length);

} // namespace DTLib {

#endif // #ifndef _HILBERT_H
