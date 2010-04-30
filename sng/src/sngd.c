/*****************************************************************************

NAME
   sngd.c -- decompile PNG to SNG.

*****************************************************************************/
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#define PNG_INTERNAL
#include "config.h"	/* for RGBTXT */
#include "png.h"
#include "sng.h"

static char *image_type[] = {
    "grayscale",
    "undefined type",
    "RGB",
    "colormap",
    "grayscale+alpha",
    "undefined type",
    "RGB+alpha"
};

static char *rendering_intent[] = {
  "perceptual",
  "relative colorimetric",
  "saturation-preserving",
  "absolute colorimetric"
};

static char *current_file;
static png_structp png_ptr;
static png_infop info_ptr;

/*****************************************************************************
 *
 * Interface to RGB database
 *
 *****************************************************************************/

#define COLOR_HASH(r, g, b)	(((r<<16)|(g<<8)|(b))%COLOR_HASH_MODULUS)

static color_item *rgb_hashbuckets[COLOR_HASH_MODULUS];
static int rgb_initialized;

static int hash_by_rgb(color_item *cp)
/* hash by color's RGB value */
{
    return(COLOR_HASH(cp->r, cp->g, cp->b));
}

static char *find_by_rgb(int r, int g, int b)
{
    color_item sc, *hp;

   sc.r = r; sc.g = g; sc.b = b;

    for (hp = rgb_hashbuckets[hash_by_rgb(&sc)]; hp; hp = hp->next)
	if (hp->r == r && hp->g == g && hp->b == b)
	    return(hp->name);

    return((char *)NULL);
}

/*****************************************************************************
 *
 * Low-level helper code
 *
 *****************************************************************************/

char *safeprint(const char *buf)
/* visibilize a given string -- inverse of sngc.c:escapes() */
{
    static char vbuf[PNG_STRING_MAX_LENGTH*4+1];
    char *tp = vbuf;

    while (*buf)
    {
	if (*buf == '"')
	{
	    *tp++ = '\\'; *tp++ = '"';
	    buf++;
	}
	else if (*buf == '\\')
	{
	    *tp++ = '\\'; *tp++ = '\\';
	    buf++;
	}
	else if (isprint(*buf) || *buf == ' ')
	    *tp++ = *buf++;
	else if (*buf == '\n')
	{
	    *tp++ = '\\'; *tp++ = 'n';
	    buf++;
	}
	else if (*buf == '\r')
	{
	    *tp++ = '\\'; *tp++ = 'r';
	    buf++;
	}
	else if (*buf == '\b')
	{
	    *tp++ = '\\'; *tp++ = 'b';
	    buf++;
	}
	else if ((unsigned char) *buf < ' ')
	{
	    *tp++ = '\\'; *tp++ = '^'; *tp++ = '@' + *buf;
	    buf++;
	}
	else
	{
	    (void) sprintf(tp, "\\x%02x", (unsigned char) *buf++);
	    tp += strlen(tp);
	}
    }
    *tp++ = '\0';
    return(vbuf);
}

static void multi_dump(FILE *fpout, char *leader,
		       int width, int height,
		       unsigned char *data[])
/* dump data in a recompilable form */
{
    unsigned char *cp;
    int i, all_printable = 1, base64 = 1;

#define SHORT_DATA	50

    for (i = 0; i < height; i++)
	for (cp = data[i]; cp < data[i] + width; cp++)
	{
	    if (!isprint(*cp) && !isspace(*cp))
		all_printable = 0;
	    if (*cp > 64)
		base64 = 0;
	}

    for (i = 0; i < height; i++)
    {
	if (all_printable)
	{
	    unsigned char *cp;

	    if (i == 0)
	    {
		fprintf(fpout, "%s ", leader);
		if (height == 1 && width < SHORT_DATA)
		    fprintf(fpout, " ");
		else
		    fprintf(fpout, "\n");
	    }

	    fputc('"', fpout);
	    for (cp = data[i]; cp < data[i] + width; cp++)
	    {
		char	cbuf[2];

		cbuf[0] = *cp;
		cbuf[1] = '\0';
		fputs(safeprint(cbuf), fpout); 

		if (*cp == '\n' && cp < data[i] + width - 1)
		    fprintf(fpout, "\"\n\"");
	    }
	    fprintf(fpout, "\"%c\n", height == 1 ? ';' : ' ');
	}
	else if (base64)
	{
	    if (i == 0)
	    {
		fprintf(fpout, "%sbase64", leader);
		if (height == 1 && width < SHORT_DATA)
		    fprintf(fpout, " ");
		else
		    fprintf(fpout, "\n");
	    }
	    for (cp = data[i]; cp < data[i] + width; cp++)
		fputc(BASE64[*cp], fpout);
	    if (height == 1)
		fprintf(fpout, ";\n");
	    else
		fprintf(fpout, "\n");
	}
	else
	{
	    if (i == 0)
	    {
		fprintf(fpout, "%shex", leader);
		if (height == 1 && width < SHORT_DATA)
		    fprintf(fpout, " ");
		else
		    fprintf(fpout, "\n");
	    }
	    for (cp = data[i]; cp < data[i] + width; cp++)
	    {
		fprintf(fpout, "%02x", *cp & 0xff);

		/* only insert spacers for 8-bit images if > 1 channel */
		if (png_ptr->bit_depth == 8 && info_ptr->channels > 1)
		{
		    if (((cp - data[i]) % info_ptr->channels) == info_ptr->channels - 1)
			fputc(' ', fpout);
		}
		else if (png_ptr->bit_depth == 16)
		    if (((cp - data[i]) % (info_ptr->channels*2)) == info_ptr->channels*2-1)
			fputc(' ', fpout);
	    }
	    if (height == 1)
		fprintf(fpout, ";\n");
	    else
		fprintf(fpout, "\n");
	}
    }
}

static void dump_data(FILE *fpout, char *leader, int size, unsigned char *data)
{
    unsigned char *dope[1];

    dope[0] = data;
    multi_dump(fpout, leader, size, 1, dope);
}

static void printerr(int err, const char *fmt, ... )
/* throw an error distinguishable from PNG library errors */
{
    char buf[BUFSIZ];
    va_list ap;

    sprintf(buf, "sng: in %s, ", current_file);

    va_start(ap, fmt);
    vsprintf(buf + strlen(buf), fmt, ap);
    va_end(ap);

    strcat(buf, "\n");
    fputs(buf, stderr);

    sng_error = err;
}

/*****************************************************************************
 *
 * Chunk handlers
 *
 *****************************************************************************/

static void dump_IHDR(FILE *fpout)
{
    int ityp;

    if (info_ptr->width == 0 || info_ptr->height == 0) {
	printerr(1, "invalid IHDR image dimensions (%lux%lu)",
		 info_ptr->width, info_ptr->height);
    }

    ityp = info_ptr->color_type;
    if (ityp > sizeof(image_type)/sizeof(char*)) {
	ityp = 1; /* avoid out of range array index */
    }
    switch (info_ptr->bit_depth) {
    case 1:
    case 2:
    case 4:
	if (ityp == 2 || ityp == 4 || ityp == 6) {/* RGB or GA or RGBA */
	    printerr(1, "invalid IHDR bit depth (%u) for %s image",
		     png_ptr->bit_depth, image_type[ityp]);
	}
	break;
    case 8:
	break;
    case 16:
	if (ityp == 3) { /* palette */
	    printerr(1, "invalid IHDR bit depth (%u) for %s image",
		     png_ptr->bit_depth, image_type[ityp]);
	}
	break;
    default:
	printerr(1, "invalid IHDR bit depth (%u)", info_ptr->bit_depth);
	break;
    }

    fprintf(fpout, "IHDR {\n");
    fprintf(fpout, "    width: %lu; height: %lu; bitdepth: %u;\n", 
	    info_ptr->width, info_ptr->height, png_ptr->bit_depth);
    fprintf(fpout, "    using");
    if (ityp & PNG_COLOR_MASK_COLOR)
	fprintf(fpout, " color");
    else
	fprintf(fpout, " grayscale");
    if (ityp & PNG_COLOR_MASK_PALETTE)
	fprintf(fpout, " palette");
    if (ityp & PNG_COLOR_MASK_ALPHA)
	fprintf(fpout, " alpha");
    fprintf(fpout, ";\n");
    if (info_ptr->interlace_type)
	fprintf(fpout, "    with interlace;        # type adam7 assumed\n");
    fprintf(fpout, "}\n");
}

static void dump_PLTE(FILE *fpout)
{
    int i;

    initialize_hash(hash_by_rgb, rgb_hashbuckets, &rgb_initialized);

    if (info_ptr->color_type & PNG_COLOR_MASK_PALETTE)
    {
	fprintf(fpout, "PLTE {\n");
#ifndef MNG_INTERFACE
	for (i = 0;  i < info_ptr->num_palette;  i++)
#else
	for (i = 0;  i < info_ptr->palette.size;  i++)
#endif /* GUG */
	{
	    char	*name = NULL;

	    fprintf(fpout, 
		    "    (%3u,%3u,%3u)     # rgb = (0x%02x,0x%02x,0x%02x)",
#ifndef MNG_INTERFACE
		    info_ptr->palette[i].red,
		    info_ptr->palette[i].green,
		    info_ptr->palette[i].blue,
		    info_ptr->palette[i].red,
		    info_ptr->palette[i].green,
		    info_ptr->palette[i].blue);
#else
		    info_ptr->palette.colors[i].red,
		    info_ptr->palette.colors[i].green,
		    info_ptr->palette.colors[i].blue,
		    info_ptr->palette.colors[i].red,
		    info_ptr->palette.colors[i].green,
		    info_ptr->palette.colors[i].blue);
#endif /* GUG */

	    if (rgb_initialized)
#ifndef MNG_INTERFACE
		name = find_by_rgb(info_ptr->palette[i].red,
				   info_ptr->palette[i].green,
				   info_ptr->palette[i].blue);
#else
		name = find_by_rgb(info_ptr->palette.colors[i].red,
				   info_ptr->palette.colors[i].green,
				   info_ptr->palette.colors[i].blue);
#endif /* GUG */
	    if (name)
		fprintf(fpout, " %s", name);
	    fputc('\n', fpout);
	}

	fprintf(fpout, "}\n");
    }
}

static void dump_image(png_bytepp rows, FILE *fpout)
{
    if (idat)
    {
	int	i;

	for (i = 0; i < idat; i++)
	{
	    fprintf(fpout, "IDAT {\n");
	    fprintf(fpout, "}\n");
	}
    }
    else
    {
	fprintf(fpout, "IMAGE {\n");
	multi_dump(fpout, "    pixels ", 
		   png_get_rowbytes(png_ptr, info_ptr),  info_ptr->height,
		   rows);
	fprintf(fpout, "}\n");
    }
}

static void dump_bKGD(FILE *fpout)
{
    if (info_ptr->valid & PNG_INFO_bKGD)
    {
	fprintf(fpout, "bKGD {");
	switch (info_ptr->color_type) {
	case PNG_COLOR_TYPE_GRAY:
	case PNG_COLOR_TYPE_GRAY_ALPHA:
	    fprintf(fpout, "gray: %u;", info_ptr->background.gray);
	    break;
	case PNG_COLOR_TYPE_RGB:
	case PNG_COLOR_TYPE_RGB_ALPHA:
	    fprintf(fpout, "red: %u;  green: %u;  blue: %u;",
			info_ptr->background.red,
			info_ptr->background.green,
			info_ptr->background.blue);
	    break;
	case PNG_COLOR_TYPE_PALETTE:
	    fprintf(fpout, "index: %u", info_ptr->background.index);
	    break;
	default:
	    printerr(1, "unknown image type");
	}
	fprintf(fpout, "}\n");
    }
}

static void dump_cHRM(FILE *fpout)
{
    double wx, wy, rx, ry, gx, gy, bx, by;


#ifdef PNG_FLOATING_POINT_SUPPORTED
#ifndef MNG_INTERFACE
    wx = info_ptr->x_white;
    wy = info_ptr->y_white;
    rx = info_ptr->x_red;
    ry = info_ptr->y_red;
    gx = info_ptr->x_green;
    gy = info_ptr->y_green;
    bx = info_ptr->x_blue;
    by = info_ptr->y_blue;
#else
    wx = info_ptr->chrm.x_white;
    wy = info_ptr->chrm.y_white;
    rx = info_ptr->chrm.x_red;
    ry = info_ptr->chrm.y_red;
    gx = info_ptr->chrm.x_green;
    gy = info_ptr->chrm.y_green;
    bx = info_ptr->chrm.x_blue;
    by = info_ptr->chrm.y_blue;
#endif /* MNG_INTERFACE */
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
#ifndef MNG_INTERFACE
    wx = FIXED_TO_FLOAT(info_ptr->int_x_white);
    wy = FIXED_TO_FLOAT(info_ptr->int_y_white);
    rx = FIXED_TO_FLOAT(info_ptr->int_x_red);
    ry = FIXED_TO_FLOAT(info_ptr->int_y_red);
    gx = FIXED_TO_FLOAT(info_ptr->int_x_green);
    gy = FIXED_TO_FLOAT(info_ptr->int_y_green);
    bx = FIXED_TO_FLOAT(info_ptr->int_x_blue);
    by = FIXED_TO_FLOAT(info_ptr->int_y_blue);
#else
    wx = FIXED_TO_FLOAT(info_ptr->chrm.int_x_white);
    wy = FIXED_TO_FLOAT(info_ptr->chrm.int_y_white);
    rx = FIXED_TO_FLOAT(info_ptr->chrm.int_x_red);
    ry = FIXED_TO_FLOAT(info_ptr->chrm.int_y_red);
    gx = FIXED_TO_FLOAT(info_ptr->chrm.int_x_green);
    gy = FIXED_TO_FLOAT(info_ptr->chrm.int_y_green);
    bx = FIXED_TO_FLOAT(info_ptr->chrm.int_x_blue);
    by = FIXED_TO_FLOAT(info_ptr->chrm.int_y_blue);
#endif /* MNG_INTERFACE */
#endif
#endif

    if (wx < 0 || wx > 0.8 || wy < 0 || wy > 0.8 || wx + wy > 1.0) {
	printerr(1, "invalid cHRM white point %0g %0g", wx, wy);
    } else if (rx < 0 || rx > 0.8 || ry < 0 || ry > 0.8 || rx + ry > 1.0) {
	printerr(1, "invalid cHRM red point %0g %0g", rx, ry);
    } else if (gx < 0 || gx > 0.8 || gy < 0 || gy > 0.8 || gx + gy > 1.0) {
	printerr(1, "invalid cHRM green point %0g %0g", gx, gy);
    } else if (bx < 0 || bx > 0.8 || by < 0 || by > 0.8 || bx + by > 1.0) {
	printerr(1, "invalid cHRM blue point %0g %0g", bx, by);
    }

    if (info_ptr->valid & PNG_INFO_cHRM) {
	fprintf(fpout, "cHRM {\n");
	fprintf(fpout, "    white: (%0g, %0g);\n", wx, wy);
	fprintf(fpout, "    red:   (%0g, %0g);\n", rx, ry);
	fprintf(fpout, "    green: (%0g, %0g);\n", gx, gy);
	fprintf(fpout, "    blue:  (%0g, %0g);\n", bx, by);
	fprintf(fpout, "}\n");
    }
}

static void dump_gAMA(FILE *fpout)
{
    if (info_ptr->valid & PNG_INFO_gAMA) {
#ifdef PNG_FLOATING_POINT_SUPPORTED
        fprintf(fpout, "gAMA {%#0.5g}\n",
#ifndef MNG_INTERFACE
		info_ptr->gamma);
#else
		info_ptr->gamma.float_gamma);
#endif /* MNG_INTERFACE */
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
        fprintf(fpout, "gAMA {%#0.5g}\n",
#ifndef MNG_INTERFACE
		FIXED_TO_FLOAT(info_ptr->int_gamma));
#else
		FIXED_TO_FLOAT(info_ptr->gamma.int_gamma));
#endif /* MNG_INTERFACE */
#endif
#endif
    }
}

static void dump_hIST(FILE *fpout)
{
    if (info_ptr->valid & PNG_INFO_hIST) {
	int	j;

	fprintf(fpout, "hIST {\n");
	fprintf(fpout, "   ");
#ifndef MNG_INTERFACE
	for (j = 0; j < info_ptr->num_palette;  j++)
	    fprintf(fpout, " %3u", info_ptr->hist[j]);
#else
	for (j = 0; j < info_ptr->palette.size;  j++)
	    fprintf(fpout, " %3u", info_ptr->hist.frequencies[j]);
#endif /* GUG */
	fprintf(fpout, ";\n}\n");
    }
}

static void dump_iCCP(FILE *fpout)
{
    if (info_ptr->valid & PNG_INFO_iCCP) {
	fprintf(fpout, "iCCP {\n");
#ifndef MNG_INTERFACE
	fprintf(fpout, "    name: \"%s\"\n", safeprint(info_ptr->iccp_name));
#else
	fprintf(fpout, "    name: \"%s\"\n", safeprint(info_ptr->iccp.name));
#endif /* MNG_INTERFACE */
	dump_data(fpout, "    profile: ", 
#ifndef MNG_INTERFACE
		  info_ptr->iccp_proflen, info_ptr->iccp_profile);
#else
		  info_ptr->iccp.proflen, info_ptr->iccp.profile);
#endif /* MNG_INTERFACE */
	fprintf(fpout, "}\n");
    }
}

static void dump_oFFs(FILE *fpout)
{
    if (info_ptr->valid & PNG_INFO_oFFs) {
        fprintf(fpout, "oFFs {xoffset: %ld; yoffset: %ld;",
#ifndef MNG_INTERFACE
	       info_ptr->x_offset, info_ptr->y_offset);
	if (info_ptr->offset_unit_type == PNG_OFFSET_PIXEL)
#else
	       info_ptr->offset.x, info_ptr->offset.y);
	if (info_ptr->offset.unit_type == PNG_OFFSET_PIXEL)
#endif /* MNG_INTERFACE */
	    fprintf(fpout, " unit: pixels");
#ifndef MNG_INTERFACE
	else if (info_ptr->offset_unit_type == PNG_OFFSET_MICROMETER)
#else
	else if (info_ptr->offset.unit_type == PNG_OFFSET_MICROMETER)
#endif /* MNG_INTERFACE */
	    fprintf(fpout, " unit: micrometers");
	fprintf(fpout, ";}\n");
    }
}
static void dump_pHYs(FILE *fpout)
{
#ifndef MNG_INTERFACE
    if (info_ptr->phys_unit_type > 1)
#else
    if (info_ptr->phys.unit_type > 1)
#endif /* MNG_INTERFACE */
        printerr(1, "invalid pHYs unit");
    else if (info_ptr->valid & PNG_INFO_pHYs) {
        fprintf(fpout, "pHYs {xpixels: %lu; ypixels: %lu;",
#ifndef MNG_INTERFACE
	       info_ptr->x_pixels_per_unit, info_ptr->y_pixels_per_unit);
	if (info_ptr->phys_unit_type == PNG_RESOLUTION_METER)
#else
	       info_ptr->phys.x_pixels_per_unit, info_ptr->phys.y_pixels_per_unit);
	if (info_ptr->phys.unit_type == PNG_RESOLUTION_METER)
#endif /* MNG_INTERFACE */
	    fprintf(fpout, " per: meter;");
        fprintf(fpout, "}");
#ifndef MNG_INTERFACE
        if (info_ptr->phys_unit_type == 1 && info_ptr->x_pixels_per_unit == info_ptr->y_pixels_per_unit)
	    fprintf(fpout, "  # (%lu dpi)\n", (long)(info_ptr->x_pixels_per_unit*0.0254 + 0.5));
#else
        if (info_ptr->phys.unit_type == 1 && info_ptr->phys.x_pixels_per_unit == info_ptr->phys.y_pixels_per_unit)
	    fprintf(fpout, "  # (%lu dpi)\n", (long)(info_ptr->phys.x_pixels_per_unit*0.0254 + 0.5));
#endif /* MNG_INTERFACE */
	else
	    fputc('\n', fpout);
    }
}

static void dump_sBIT(FILE *fpout)
{
    int maxbits = (info_ptr->color_type == 3)? 8 : info_ptr->bit_depth;

    if (info_ptr->valid & PNG_INFO_sBIT) {
	fprintf(fpout, "sBIT {\n");
	switch (info_ptr->color_type) {
	case PNG_COLOR_TYPE_GRAY:
	    if (info_ptr->sig_bit.gray == 0 || info_ptr->sig_bit.gray > maxbits) {
		printerr(1, "%u sBIT gray bits not valid for %ubit/sample image",
			 info_ptr->sig_bit.gray, maxbits);
	    } else {
		fprintf(fpout, "    gray: %u;\n", info_ptr->sig_bit.gray);
	    }
	    break;
	case PNG_COLOR_TYPE_RGB:
	case PNG_COLOR_TYPE_PALETTE:
	    if (info_ptr->sig_bit.red == 0 || info_ptr->sig_bit.red > maxbits) {
		printerr(1, "%u sBIT red bits not valid for %ubit/sample image",
			 info_ptr->sig_bit.red, maxbits);
	    } else if (info_ptr->sig_bit.green == 0 || info_ptr->sig_bit.green > maxbits) {
		printerr(1, "%u sBIT green bits not valid for %ubit/sample image",
			 info_ptr->sig_bit.green, maxbits);
	    } else if (info_ptr->sig_bit.blue == 0 || info_ptr->sig_bit.blue > maxbits) {
		printerr(1, "%u sBIT blue bits not valid for %ubit/sample image",
			 info_ptr->sig_bit.blue, maxbits);
	    } else {
		fprintf(fpout, "    red: %u; green: %u; blue: %u;\n",
			info_ptr->sig_bit.red, info_ptr->sig_bit.green, info_ptr->sig_bit.blue);
	    }
	    break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
	    if (info_ptr->sig_bit.gray == 0 || info_ptr->sig_bit.gray > maxbits) {
		printerr(2, "%u sBIT gray bits not valid for %ubit/sample image\n",
			 info_ptr->sig_bit.gray, maxbits);
	    } else if (info_ptr->sig_bit.alpha == 0 || info_ptr->sig_bit.alpha > maxbits) {
		printerr(2, "%u sBIT alpha bits(tm) not valid for %ubit/sample image\n",
			 info_ptr->sig_bit.alpha, maxbits);
	    } else {
		fprintf(fpout, "    gray: %u; alpha: %u\n", info_ptr->sig_bit.gray, info_ptr->sig_bit.alpha);
	    }
	    break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
	    if (info_ptr->sig_bit.gray == 0 || info_ptr->sig_bit.gray > maxbits) {
		printerr(1, "%u sBIT red bits not valid for %ubit/sample image",
			 info_ptr->sig_bit.gray, maxbits);
	    } else if (info_ptr->sig_bit.green == 0 || info_ptr->sig_bit.green > maxbits) {
		printerr(1, "%u sBIT green bits not valid for %ubit/sample image",
			 info_ptr->sig_bit.green, maxbits);
	    } else if (info_ptr->sig_bit.blue == 0 || info_ptr->sig_bit.blue > maxbits) {
		printerr(1, "%u sBIT blue bits not valid for %ubit/sample image",
			 info_ptr->sig_bit.blue, maxbits);
	    } else if (info_ptr->sig_bit.alpha == 0 || info_ptr->sig_bit.alpha > maxbits) {
		printerr(1, "%u sBIT alpha bits not valid for %ubit/sample image",
			 info_ptr->sig_bit.alpha, maxbits);
	    } else {
		fprintf(fpout, "    red: %u; green: %u; blue: %u; alpha: %u;\n",
			info_ptr->sig_bit.red, 
			info_ptr->sig_bit.green,
			info_ptr->sig_bit.blue,
			info_ptr->sig_bit.alpha);
	    }
	    break;
	}
	fprintf(fpout, "}\n");
    }
}

static void dump_pCAL(FILE *fpout)
{
    static char *mapping_type[] = {
	"linear", "euler", "exponential", "hyperbolic"
    }; 

#ifndef MNG_INTERFACE
    if (info_ptr->pcal_type >= PNG_EQUATION_LAST)
#else
    if (info_ptr->pcal.type >= PNG_EQUATION_LAST)
#endif /* MNG_INTERFACE */
	    printerr(1, "invalid equation type in pCAL");
    else if (info_ptr->valid & PNG_INFO_pCAL) {
	int	i;

	fprintf(fpout, "pCAL {\n");
#ifndef MNG_INTERFACE
	fprintf(fpout, "    name: \"%s\";\n", safeprint(info_ptr->pcal_purpose));
	fprintf(fpout, "    x0: %ld;\n", info_ptr->pcal_X0);
	fprintf(fpout, "    x1: %ld;\n", info_ptr->pcal_X1);
#else
	fprintf(fpout, "    name: \"%s\";\n", safeprint(info_ptr->pcal.purpose));
	fprintf(fpout, "    x0: %ld;\n", info_ptr->pcal.X0);
	fprintf(fpout, "    x1: %ld;\n", info_ptr->pcal.X1);
#endif /* MNG_INTERFACE */
	fprintf(fpout, "    mapping: %s;        # equation type %u\n", 
#ifndef MNG_INTERFACE
	       mapping_type[info_ptr->pcal_type], info_ptr->pcal_type);
	fprintf(fpout, "    unit: \"%s\"\n", safeprint(info_ptr->pcal_units));
	if (info_ptr->pcal_nparams)
#else
	       mapping_type[info_ptr->pcal.type], info_ptr->pcal.type);
	fprintf(fpout, "    unit: \"%s\"\n", safeprint(info_ptr->pcal.units));
	if (info_ptr->pcal.nparams)
#endif /* MNG_INTERFACE */
	{
	    fprintf(fpout, "    parameters:");
#ifndef MNG_INTERFACE
	    for (i = 0; i < info_ptr->pcal_nparams; i++)
		fprintf(fpout, " %s", safeprint(info_ptr->pcal_params[i]));
#else
	    for (i = 0; i < info_ptr->pcal.nparams; i++)
		fprintf(fpout, " %s", safeprint(info_ptr->pcal.params[i]));
#endif /* MNG_INTERFACE */
	    fprintf(fpout, ";\n");
	}
	fprintf(fpout, "}\n");
    }
}

static void dump_sCAL(FILE *fpout)
{
    if (info_ptr->valid & PNG_INFO_sCAL) {
	fprintf(fpout, "sCAL {\n");
#ifndef MNG_INTERFACE
	switch (info_ptr->scal_unit)
#else
	switch (info_ptr->scal.unit)
#endif /* MNG_INTERFACE */
	{
	case PNG_SCALE_METER:
	    fprintf(fpout, "    unit:   meter\n");
	    break;
	case PNG_SCALE_RADIAN:
	    fprintf(fpout, "    unit:   radian\n");
	    break;
	default:
	    fprintf(fpout, "    unit:   unknown\n");
	    break;
	}
#ifdef PNG_FLOATING_POINT_SUPPORTED
#ifndef MNG_INTERFACE
	fprintf(fpout, "    width:  %g\n", info_ptr->scal_pixel_width);
	fprintf(fpout, "    height: %g\n", info_ptr->scal_pixel_height);
#else
	fprintf(fpout, "    width:  %g\n", info_ptr->scal.pixel_width);
	fprintf(fpout, "    height: %g\n", info_ptr->scal.pixel_height);
#endif /* MNG_INTERFACE */
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
#ifndef MNG_INTERFACE
	fprintf(fpout, "    width:  %s\n", info_ptr->scal_s_width);
	fprintf(fpout, "    height: %s\n", info_ptr->scal_s_height);
#else
	fprintf(fpout, "    width:  %s\n", info_ptr->scal.s_width);
	fprintf(fpout, "    height: %s\n", info_ptr->scal.s_height);
#endif /* MNG_INTERFACE */
#endif
#endif
	fprintf(fpout, "}\n");
    }
}

static void dump_sPLT(png_sPLT_tp ep, FILE *fpout)
{
    long i;

    initialize_hash(hash_by_rgb, rgb_hashbuckets, &rgb_initialized);

    fprintf(fpout, "sPLT {\n");
    fprintf(fpout, "    name: \"%s\";\n", safeprint(ep->name));
    fprintf(fpout, "    depth: %u;\n", ep->depth);

    for (i = 0;  i < ep->nentries;  i++)
    {
	char *name = 0;

	fprintf(fpout, "    (%3u,%3u,%3u), %3u, %3u "
		"    # rgba = [0x%02x,0x%02x,0x%02x,0x%02x]",
		ep->entries[i].red,
		ep->entries[i].green,
		ep->entries[i].blue,
		ep->entries[i].alpha,
		ep->entries[i].frequency,
		ep->entries[i].red,
		ep->entries[i].green,
		ep->entries[i].blue,
		ep->entries[i].alpha);

	if (rgb_initialized)
	    name = find_by_rgb(ep->entries[i].red,
			       ep->entries[i].green,
			       ep->entries[i].blue);
	if (name)
	    fprintf(fpout, ", name = %s", name);

	fprintf(fpout, ", freq = %u\n", ep->entries[i].frequency);
    }

    fprintf(fpout, "}\n");
}

static void dump_tRNS(FILE *fpout)
{
    if (info_ptr->valid & PNG_INFO_tRNS) {
	int	i;

	fprintf(fpout, "tRNS {\n");
	switch (info_ptr->color_type) {
	case PNG_COLOR_TYPE_GRAY:
	    fprintf(fpout, "    gray: %u;\n", info_ptr->trans_values.gray);
	    break;
	case PNG_COLOR_TYPE_RGB:
	    fprintf(fpout, "    red: %u; green: %u; blue: %u;\n",
		    info_ptr->trans_values.red,
		    info_ptr->trans_values.green,
		    info_ptr->trans_values.blue);
	    break;
	case PNG_COLOR_TYPE_PALETTE:
	    for (i = 0; i < info_ptr->num_trans; i++)
		fprintf(fpout, " %u", info_ptr->trans[i]);
	    break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
	case PNG_COLOR_TYPE_RGB_ALPHA:
	    printerr(1, "tRNS chunk illegal with this image type");
	    break;
	}
	fprintf(fpout, "}\n");
    }
}

static void dump_sRGB(FILE *fpout)
{
#ifndef MNG_INTERFACE
    if (info_ptr->srgb_intent) {
#else
    if (info_ptr->srgb.intent) {
#endif /* MNG_INTERFACE */
        printerr(1, "sRGB invalid rendering intent");
    }
    if (info_ptr->valid & PNG_INFO_sRGB) {
        fprintf(fpout, "sRGB {intent: %u;}             # %s\n", 
#ifndef MNG_INTERFACE
	       info_ptr->srgb_intent, 
	       rendering_intent[info_ptr->srgb_intent]);
#else
	       info_ptr->srgb.intent, 
	       rendering_intent[info_ptr->srgb.intent]);
#endif /* MNG_INTERFACE */
    }
}

static void dump_tIME(FILE *fpout)
{
    if (info_ptr->valid & PNG_INFO_tIME) {
	static char *months[] =
	{"(undefined)", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
	 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	char *month;

	if (info_ptr->mod_time.month < 1 || info_ptr->mod_time.month > 12)
	    month = months[0];
	else
	    month = months[info_ptr->mod_time.month];

	fprintf(fpout, "tIME {\n");
	fprintf(fpout, "    # %2d %s %4d %02d:%02d:%02d GMT\n", 
		info_ptr->mod_time.day,
		month,
		info_ptr->mod_time.year,
		info_ptr->mod_time.hour,
		info_ptr->mod_time.minute,
		info_ptr->mod_time.second);
	fprintf(fpout, "    year:   %u\n", info_ptr->mod_time.year);
	fprintf(fpout, "    month:  %u\n", info_ptr->mod_time.month);
	fprintf(fpout, "    day:    %u\n", info_ptr->mod_time.day);
	fprintf(fpout, "    hour:   %u\n", info_ptr->mod_time.hour);
	fprintf(fpout, "    minute: %u\n", info_ptr->mod_time.minute);
	fprintf(fpout, "    second: %u\n", info_ptr->mod_time.second);
	fprintf(fpout, "}\n");
    }
}

static void dump_text(FILE *fpout)
{
    int	i;

#ifndef MNG_INTERFACE
    for (i = 0; i < info_ptr->num_text; i++)
#else
    for (i = 0; i < info_ptr->annotations.num_text; i++)
#endif /* MNG_INTERFACE */
    {
#ifndef MNG_INTERFACE
	switch (info_ptr->text[i].compression)
#else
	switch (info_ptr->annotations.text[i].compression)
#endif /* MNG_INTERFACE */
	{
	case PNG_TEXT_COMPRESSION_NONE:
	    fprintf(fpout, "tEXt {\n");
	    fprintf(fpout, "    keyword: \"%s\";\n", 
#ifndef MNG_INTERFACE
		    safeprint(info_ptr->text[i].key));
#else
		    safeprint(info_ptr->annotations.text[i].key));
#endif /* MNG_INTERFACE */
	    break;

	case PNG_TEXT_COMPRESSION_zTXt:
	    fprintf(fpout, "zTXt {\n");
	    fprintf(fpout, "    keyword: \"%s\";\n", 
#ifndef MNG_INTERFACE
		    safeprint(info_ptr->text[i].key));
#else
		    safeprint(info_ptr->annotations.text[i].key));
#endif /* MNG_INTERFACE */
	    break;

	case PNG_ITXT_COMPRESSION_NONE:
	case PNG_ITXT_COMPRESSION_zTXt:
	    fprintf(fpout, "iTXt {\n");
#ifdef PNG_iTXt_SUPPORTED
	    fprintf(fpout, "    language: \"%s\";\n", 
#ifndef MNG_INTERFACE
		    safeprint(info_ptr->text[i].lang));
#else
		    safeprint(info_ptr->annotations.text[i].lang));
#endif /* MNG_INTERFACE */
#endif /* PNG_iTXt_SUPPORTED */
	    fprintf(fpout, "    keyword: \"%s\";\n", 
#ifndef MNG_INTERFACE
		    safeprint(info_ptr->text[i].key));
#else
		    safeprint(info_ptr->annotations.text[i].key));
#endif /* MNG_INTERFACE */
#ifdef PNG_iTXt_SUPPORTED
	    fprintf(fpout, "    translated: \"%s\";\n", 
#ifndef MNG_INTERFACE
		    safeprint(info_ptr->text[i].lang_key));
#else
		    safeprint(info_ptr->annotations.text[i].lang_key));
#endif /* MNG_INTERFACE */
#endif /* PNG_iTXt_SUPPORTED */
	    break;
	}

	fprintf(fpout, "    text: \"%s\";\n", 
#ifndef MNG_INTERFACE
		safeprint(info_ptr->text[i].text));
#else
		safeprint(info_ptr->annotations.text[i].text));
#endif /* MNG_INTERFACE */
	fprintf(fpout, "}\n");
    }
}

static void dump_unknown_chunks(int after_idat, FILE *fpout)
{
    int	i;

    for (i = 0; i < info_ptr->unknown_chunks_num; i++)
    {
	png_unknown_chunk	*up = info_ptr->unknown_chunks + i;

	/* are we before or after the IDAT part? */
	if (after_idat != !!(up->location & PNG_HAVE_IDAT))
	    continue;

/* macros to extract big-endian short and long ints */
#define SH(p) ((unsigned short)((p)[1]) | (((p)[0]) << 8))
#define LG(p) ((unsigned long)(SH((p)+2)) | ((ulg)(SH(p)) << 16))

	if (!strcmp(up->name, "gIFg"))
	{
	  fprintf(fpout, "gIFg {\n");
	    fprintf(fpout, "    disposal: %d; input: %d; delay %f;\n",
		    up->data[0], up->data[1], (float)(.01 * SH(up->data+2)));
	}
	else if (!strcmp(up->name, "gIFx"))
	{
	    fprintf(fpout, "gIFx {\n");
	    fprintf(fpout, "    identifier: \"%.*s\"; code: \"%c%c%c\"\n",
		    8, up->data, up->data[8], up->data[9], up->data[10]);
	    dump_data(fpout, "    data: ", up->size - 11, up->data + 11);
	}
	else
	{
	    fprintf(fpout, "private %s {\n", up->name);
	    dump_data(fpout, "    ", up->size, up->data);
	}
	fprintf(fpout, "}\n");
#undef SH
#undef LG
    }
}

/*****************************************************************************
 *
 * Compiler main sequence
 *
 *****************************************************************************/

void sngdump(png_byte *row_pointers[], FILE *fpout)
/* dump a canonicalized SNG form of a PNG file */
{
    int	i;

    fprintf(fpout, "#SNG: from %s\n", current_file);

    dump_IHDR(fpout);			/* first critical chunk */

    dump_cHRM(fpout);
    dump_gAMA(fpout);
    dump_iCCP(fpout);
    dump_sBIT(fpout);
    dump_sRGB(fpout);

    dump_PLTE(fpout);			/* second critical chunk */

    dump_bKGD(fpout);
    dump_hIST(fpout);
    dump_tRNS(fpout);
    dump_pHYs(fpout);
#ifndef MNG_INTERFACE
    for (i = 0; i < info_ptr->splt_palettes_num; i++)
	dump_sPLT(info_ptr->splt_palettes + i, fpout);
#else
    for (i = 0; i < info_ptr->splt.palettes_num; i++)
	dump_sPLT(info_ptr->splt.palettes + i, fpout);
#endif /* MNG_INTERFACE */

    dump_unknown_chunks(FALSE, fpout);

    /*
     * This is the earliest point at which we could write the image data;
     * the ancillary chunks after this point have no order contraints.
     * We choose to write the image last so that viewers/editors can get
     * a look at all the ancillary information.
     */

    dump_tIME(fpout);
    dump_text(fpout);

    dump_image(row_pointers, fpout);	/* third critical chunk */

    dump_unknown_chunks(TRUE, fpout);
}

static int read_chunk_callback(png_structp ptr, png_unknown_chunkp chunk)
{
    /* FIXME: the -i option doesn't work yet, it yields a CRC error */
    if (memcmp(chunk->name, "IDAT", 4) == 0)
	png_ptr->mode |= PNG_HAVE_IDAT;
    return(1);
}

int sngd(FILE *fp, char *name, FILE *fpout)
/* read and decompile an SNG image presented on stdin */
{
#ifndef PNG_INFO_IMAGE_SUPPORTED
   png_bytepp row_pointers;
#endif

   current_file = name;
   sng_error = 0;

   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also supply the
    * the compiler header file version, so that we know if the application
    * was compiled with a compatible version of the library.  REQUIRED
    */
   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   if (png_ptr == NULL)
   {
      fclose(fp);
      sng_error = 1;
      return(1);
   }

   /* Allocate/initialize the memory for image information.  REQUIRED. */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fclose(fp);
      png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
      sng_error = 1;
      return(FAIL);
   }

   /* Set error handling if you are using the setjmp/longjmp method (this is
    * the normal method of doing things with libpng).  REQUIRED unless you
    * set up your own error handlers in the png_create_read_struct() earlier.
    */
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
      fclose(fp);
      /* If we get here, we had a problem reading the file */
      sng_error = 1;
      return(1);
   }

   /* keep all unknown chunks, we'll dump them later */
   png_set_keep_unknown_chunks(png_ptr, 2, NULL, 0);

   /* treat IDAT as unknown so it gets passed through raw */
   if (idat)
   {
       png_set_keep_unknown_chunks(png_ptr, 2, "IDAT", 1);
       png_set_read_user_chunk_fn(png_ptr, NULL, read_chunk_callback);
   }


   /* Set up the input control if you are using standard C streams */
   png_init_io(png_ptr, fp);


   /*
    * Unpack images with bit depth < 8 into bytes per sample.
    * We'll cheat, later on, by referring to png_ptr->bit_depth.
    * This preserves the bit depth of the file, as opposed to
    * the unpacked bit depth of the image.  But this is
    * undocumented.  If it ever breaks, the regression test
    * will start failing on images of depth 1, 2, and 4.
    */
#ifdef PNG_INFO_IMAGE_SUPPORTED
   png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_PACKING, NULL);

   /* dump the image */
   sngdump(info_ptr->row_pointers, fpout);
#else
   png_set_packing(png_ptr);

   /* The call to png_read_info() gives us all of the information from the
    * PNG file before the first IDAT (image data chunk).  REQUIRED
    */
   png_read_info(png_ptr, info_ptr);

   png_read_update_info(png_ptr, info_ptr);

   row_pointers = (png_bytepp)malloc(info_ptr->height * sizeof(png_bytep));
   for (row = 0; row < info_ptr->height; row++)
       row_pointers[row] = malloc(png_get_rowbytes(png_ptr, info_ptr));

   png_read_image(png_ptr, row_pointers);

   /* read rest of file, and get additional chunks in info_ptr - REQUIRED */
   png_read_end(png_ptr, info_ptr);

   /* dump the image */
   sngdump(row_pointers, fpout);
#endif

   /* clean up after the read, and free any memory allocated - REQUIRED */
   png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

   /* close the file */
   fclose(fp);

   /* that's it */
   return(0);
}

/* sngd.c ends here */
