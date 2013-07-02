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
// hilbert.cpp - hilbert transform
// Author: Doron Tal (this code is based on the matlab (v5.3) 'hilbert.m')
// Date created: March, 2000

#include <assert.h>
#include "hilbert.h"
#include "utils.h"
#include "fft.h"

using namespace DTLib;

/////////////////////////////////////////////////////////////////////////////
// In-place hilbert transform on aInVector.

void DTLib::Hilbert(float *aInVector, const int Length)
{
    float *aRe, *aIm;
    // convert to a power of 2 sequence to speed up fft
    const int LengthFFT = (int)pow(2.0, (double)NextPowerOfTwo(Length));
    bool bAllocatedRe; // remember so that we can free it later if necc.
    if (LengthFFT == Length) { 
        aRe = aInVector; // input is real
        aIm = new float[Length];
    }
    else { // need to enlarge & copy sequences
        bAllocatedRe = true;
        aRe = new float[LengthFFT];	assert(aRe != NULL);
        aIm = new float[LengthFFT];	assert (aIm != NULL);
        memcpy(aRe, aInVector, Length*sizeof(float)); // input is real
        memset(aRe+Length, 0, (LengthFFT-Length)*sizeof(float)); // zero-pad
    }
    memset(aIm, 0, LengthFFT*sizeof(float)); // imaginary is zero
    // convolve with Hilbert kernel in Fourier domain:
    FwdFFT(aRe, aIm, LengthFFT);
    const int nHilbertTwos = (LengthFFT/2)-1; // # of twos in hilbert kernel
    float *pRe = aRe+1, *pIm = aIm+1; // 1st elt untouched
    for (int i = 0; i < nHilbertTwos; i++, pRe++, pIm++) {
        *pRe *= 2.0f;
        *pIm *= 2.0f;
    }
    pRe++; pIm++; // next elt untouched
    memset(pRe, 0, nHilbertTwos*sizeof(float)); // the rest are zero
    memset(pIm, 0, nHilbertTwos*sizeof(float));
    InvFFT(aRe, aIm, LengthFFT);
    // copy over the result (aIm) into aInVector
    memcpy(aInVector, aIm, Length*sizeof(float));
    zap(aRe);
    zap(aIm);
}
