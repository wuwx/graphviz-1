/* $Id$ $Revision$ */
/* vim:set shiftwidth=4 ts=8: */

/**********************************************************
*      This software is part of the graphviz package      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2004 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
**********************************************************/

#include "render.h"
#include "utils.h"

#ifdef HAVE_CAIRO
#include <cairo.h>
#else

static double timesFontWidth[] = {
    0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500,	/*          */
    0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500,	/*          */
    0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500,	/*          */
    0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500,	/*          */
    0.2500, 0.3329, 0.4079, 0.5000, 0.5000, 0.8329, 0.7779, 0.3329,	/*  !"#$%&' */
    0.3329, 0.3329, 0.5000, 0.5639, 0.2500, 0.3329, 0.2500, 0.2779,	/* ()*+,-./ */
    0.5000, 0.5000, 0.5000, 0.5000, 0.5000, 0.5000, 0.5000, 0.5000,	/* 01234567 */
    0.5000, 0.5000, 0.2779, 0.2779, 0.5639, 0.5639, 0.5639, 0.4439,	/* 89:;<=>? */
    0.9209, 0.7219, 0.6669, 0.6669, 0.7219, 0.6109, 0.5559, 0.7219,	/* @ABCDEFG */
    0.7219, 0.3329, 0.3889, 0.7219, 0.6109, 0.8889, 0.7219, 0.7219,	/* HIJKLMNO */
    0.5559, 0.7219, 0.6669, 0.5559, 0.6109, 0.7219, 0.7219, 0.9439,	/* PQRSTUVW */
    0.7219, 0.7219, 0.6109, 0.3329, 0.2779, 0.3329, 0.4689, 0.5000,	/* XYZ[\]^_ */
    0.3329, 0.4439, 0.5000, 0.4439, 0.5000, 0.4439, 0.3329, 0.5000,	/* `abcdefg */
    0.5000, 0.2779, 0.2779, 0.5000, 0.2779, 0.7779, 0.5000, 0.5000,	/* hijklmno */
    0.5000, 0.5000, 0.3329, 0.3889, 0.2779, 0.5000, 0.5000, 0.7219,	/* pqrstuvw */
    0.5000, 0.5000, 0.4439, 0.4799, 0.1999, 0.4799, 0.5409, 0.2500,	/* xyz{|}~  */
    0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500,	/*          */
    0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500,	/*          */
    0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500,	/*          */
    0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500,	/*          */
    0.2500, 0.3329, 0.5000, 0.5000, 0.1669, 0.5000, 0.5000, 0.5000,	/*  ������� */
    0.5000, 0.1799, 0.4439, 0.5000, 0.3329, 0.3329, 0.5559, 0.5559,	/* �������� */
    0.2500, 0.5000, 0.5000, 0.5000, 0.2500, 0.2500, 0.4529, 0.3499,	/*  ���� �� */
    0.3329, 0.4439, 0.4439, 0.5000, 1.0000, 1.0000, 0.2500, 0.4439,	/* ������ � */
    0.2500, 0.3329, 0.3329, 0.3329, 0.3329, 0.3329, 0.3329, 0.3329,	/*  ������� */
    0.3329, 0.2500, 0.3329, 0.3329, 0.2500, 0.3329, 0.3329, 0.3329,	/* � �� ��� */
    1.0000, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500,	/* �        */
    0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500, 0.2500,	/*          */
    0.2500, 0.8889, 0.2500, 0.2759, 0.2500, 0.2500, 0.2500, 0.2500,	/*  � �     */
    0.6109, 0.7219, 0.8889, 0.3099, 0.2500, 0.2500, 0.2500, 0.2500,	/* ����     */
    0.2500, 0.6669, 0.2500, 0.2500, 0.2500, 0.2779, 0.2500, 0.2500,	/*  �   �   */
    0.2779, 0.5000, 0.7219, 0.5000, 0.2500, 0.2500, 0.2500, 0.2500,	/* ����     */
};
static double arialFontWidth[] = {
    0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/*          */
    0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/*          */
    0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/*          */
    0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/*          */
    0.2779, 0.2779, 0.3549, 0.5559, 0.5559, 0.8889, 0.6669, 0.2209,	/*  !"#$%&' */
    0.3329, 0.3329, 0.3889, 0.5839, 0.2779, 0.3329, 0.2779, 0.2779,	/* ()*+,-./ */
    0.5559, 0.5559, 0.5559, 0.5559, 0.5559, 0.5559, 0.5559, 0.5559,	/* 01234567 */
    0.5559, 0.5559, 0.2779, 0.2779, 0.5839, 0.5839, 0.5839, 0.5559,	/* 89:;<=>? */
    1.0149, 0.6669, 0.6669, 0.7219, 0.7219, 0.6669, 0.6109, 0.7779,	/* @ABCDEFG */
    0.7219, 0.2779, 0.5000, 0.6669, 0.5559, 0.8329, 0.7219, 0.7779,	/* HIJKLMNO */
    0.6669, 0.7779, 0.7219, 0.6669, 0.6109, 0.7219, 0.6669, 0.9439,	/* PQRSTUVW */
    0.6669, 0.6669, 0.6109, 0.2779, 0.2779, 0.2779, 0.4689, 0.5559,	/* XYZ[\]^_ */
    0.2219, 0.5559, 0.5559, 0.5000, 0.5559, 0.5559, 0.2779, 0.5559,	/* `abcdefg */
    0.5559, 0.2219, 0.2219, 0.5000, 0.2219, 0.8329, 0.5559, 0.5559,	/* hijklmno */
    0.5559, 0.5559, 0.3329, 0.5000, 0.2779, 0.5559, 0.5000, 0.7219,	/* pqrstuvw */
    0.5000, 0.5000, 0.5000, 0.3339, 0.2599, 0.3339, 0.5839, 0.2779,	/* xyz{|}~  */
    0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/*          */
    0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/*          */
    0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/*          */
    0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/*          */
    0.2779, 0.3329, 0.5559, 0.5559, 0.1669, 0.5559, 0.5559, 0.5559,	/*  ������� */
    0.5559, 0.1909, 0.3329, 0.5559, 0.3329, 0.3329, 0.5000, 0.5000,	/* �������� */
    0.2779, 0.5559, 0.5559, 0.5559, 0.2779, 0.2779, 0.5369, 0.3499,	/*  ���� �� */
    0.2219, 0.3329, 0.3329, 0.5559, 1.0000, 1.0000, 0.2779, 0.6109,	/* ������ � */
    0.2779, 0.3329, 0.3329, 0.3329, 0.3329, 0.3329, 0.3329, 0.3329,	/*  ������� */
    0.3329, 0.2779, 0.3329, 0.3329, 0.2779, 0.3329, 0.3329, 0.3329,	/* � �� ��� */
    1.0000, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/* �        */
    0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/*          */
    0.2779, 1.0000, 0.2779, 0.3699, 0.2779, 0.2779, 0.2779, 0.2779,	/*  � �     */
    0.5559, 0.7779, 1.0000, 0.3649, 0.2779, 0.2779, 0.2779, 0.2779,	/* ����     */
    0.2779, 0.8889, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779, 0.2779,	/*  �   �   */
    0.2219, 0.6109, 0.9439, 0.6109, 0.2779, 0.2779, 0.2779, 0.2779,	/* ����     */
};
static double courFontWidth[] = {
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*          */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*          */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*          */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*          */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*  !"#$%&' */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* ()*+,-./ */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* 01234567 */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* 89:;<=>? */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* @ABCDEFG */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* HIJKLMNO */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* PQRSTUVW */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* XYZ[\]^_ */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* `abcdefg */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* hijklmno */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* pqrstuvw */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* xyz{|}~  */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*          */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*          */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*          */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*          */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*  ������� */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* �������� */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*  ���� �� */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* ������ � */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*  ������� */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* � �� ��� */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* �        */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*          */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*  � �     */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* ����     */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/*  �   �   */
    0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999, 0.5999,	/* ����     */
};

#if !defined(DISABLE_CODEGENS) && !defined(HAVE_GD_FREETYPE)
extern codegen_t *Output_codegen;

double _dpi;
void initDPI(graph_t * g)
{
    _dpi = GD_drawing(g)->dpi;
}

double textheight(int nlines, double fontsz)
{
    if (Output_codegen == &GD_CodeGen) {
	double fsize = (fontsz * _dpi) / POINTS_PER_INCH;	/* in pixels */
	double fontsize = builtinFontHt(fsize);
	return (nlines * (int) (fontsize * LINESPACING) *
		POINTS_PER_INCH) / _dpi;
    } else
	return nlines * (int) (fontsz * LINESPACING);
}
#endif

/* estimate_textsize:
 * Estimate width of text, for given face and size, in points.
 * Value is stored textline->width.
 * NOTE: Tables are based on a font of size 1. Need to multiply by
 * fontsize to get appropriate value.
 */
static void
estimate_textsize(textline_t * textline, char *fontname, double fontsz,
		  char **fontpath)
{
    double *Fontwidth;
    char c, *p;

    textline->width = 0.0;
    textline->xshow = NULL;
#if !defined(DISABLE_CODEGENS) && !defined(HAVE_GD_FREETYPE)
    if (Output_codegen == &GD_CodeGen) {
	int cwidth;
	double fsize = (fontsz * _dpi) / POINTS_PER_INCH;	/* in pixels */
	*fontpath = "[internal gd]";
	cwidth = builtinFontWd(fsize);
	if ((p = textline->str)) {
	    textline->width = strlen(p) * cwidth * POINTS_PER_INCH / _dpi;
	}
	return;
    } else if (!strncasecmp(fontname, "cour", 4)) {
#else
    if (!strncasecmp(fontname, "cour", 4)) {
#endif
	*fontpath = "[internal courier]";
	Fontwidth = courFontWidth;
    } else if (!strncasecmp(fontname, "arial", 5)
	       || !strncasecmp(fontname, "helvetica", 9)) {
	*fontpath = "[internal arial]";
	Fontwidth = arialFontWidth;
    } else {
	*fontpath = "[internal times]";
	Fontwidth = timesFontWidth;
    }
    if ((p = textline->str)) {
	while ((c = *p++))
	    textline->width += Fontwidth[(unsigned char) c];
	textline->width *= fontsz;
    }
}
#endif

double textwidth(textline_t * textline, char *fontname, double fontsize)
{
    char *fontpath = NULL;
#ifdef HAVE_CAIRO
    cairo_t *cr;
    cairo_text_extents_t extents;

    cr = cairo_create();
    cairo_select_font(cr, fontname, 0, 0);
    cairo_scale_font(cr, fontsize);
    cairo_text_extents(cr, textline->str, &extents);
#if 0
    cairo_destroy(cr);
#else
/* FIXME - adding this test mysteriously fixes the mysteriously destroyed cr problem */
    if (cr)
	cairo_destroy(cr);
    else
	fprintf(stderr,"fontmetrics.c:textwidth() - cr mysteriously destroyed already\n");
#endif

    textline->width = extents.width;
    textline->xshow = NULL;
    fontpath = "[cairo]";
#else
    if (gd_textsize(textline, fontname, fontsize, &fontpath))
	estimate_textsize(textline, fontname, fontsize, &fontpath);
#endif

    if (Verbose) {
	if (emit_once(fontname)) {
	    fprintf(stderr, "%s: fontname=%s fontpath=%s\n", CmdName,
		    fontname, fontpath);
	}
    }
    return textline->width;
}
