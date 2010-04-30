/*****************************************************************************

NAME
   sngc.c -- compile SNG to PNG.

*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#define PNG_INTERNAL
#include "png.h"

#include "sng.h"

extern int verbose;

typedef int	bool;
#define FALSE	0
#define TRUE	1

#define NONE	-2

typedef struct {
    char	*name;		/* name of chunk type */
    bool	multiple_ok;	/* OK to have more than one? */
    int		count;		/* how many have we seen? */
} chunkprops;

#ifndef PNG_KEYWORD_MAX_LENGTH
#define PNG_KEYWORD_MAX_LENGTH	79
#endif /* PNG_KEYWORD_MAX_LENGTH */

#define MEMORY_QUANTUM	1024
#define MAX_PARAMS	16
#define PNG_MAX_LONG	2147483647L	/* 2^31 */

/* chunk types */
static chunkprops properties[] = 
{
/*
 * The PNG 1.0 chunks, listed in order of the summary table in section 4.3.
 * IEND is not listed here because it doesn't have to appear in the file.
 */
#define IHDR	0
    {"IHDR",		FALSE,	0},
#define PLTE	1
    {"PLTE",		FALSE,	0},
#define IDAT	2
    {"IDAT",		TRUE,	0},
#define cHRM	3
    {"cHRM",		FALSE,	0},
#define gAMA	4
    {"gAMA",		FALSE,	0},
#define iCCP	5
    {"iCCP",		FALSE,	0},
#define sBIT	6
    {"sBIT",		FALSE,	0},
#define sRGB	7
    {"sRGB",		FALSE,	0},
#define bKGD	8
    {"bKGD",		FALSE,	0},
#define hIST	9
    {"hIST",		FALSE,	0},
#define tRNS	10
    {"tRNS",		FALSE,	0},
#define pHYs	11
    {"pHYs",		FALSE,	0},
#define sPLT	12
    {"sPLT",		TRUE,	0},
#define tIME	13
    {"tIME",		FALSE,	0},
#define iTXt	14
    {"iTXt",		TRUE,	0},
#define tEXt	15
    {"tEXt",		TRUE,	0},
#define zTXt	16
    {"zTXt",		TRUE,	0},
/*
 * Special-purpose chunks in PNG Extensions 1.2.0 specification.
 */
#define oFFs	17
    {"oFFs",		FALSE,	0},
#define pCAL	18
    {"pCAL",		FALSE,	0},
#define sCAL	19
    {"sCAL",		FALSE,	0},
#define gIFg	20
    {"gIFg",		FALSE,	0},
#define gIFt	21
    {"gIFt",		FALSE,	0},
#define gIFx	22
    {"gIFx",		FALSE,	0},
#define fRAc	23
    {"fRAc",		FALSE,	0},

/*
 * Image pseudo-chunk
 */
#define IMAGE	24
    {"IMAGE",		FALSE,	0},

/*
 * Private chunks
 */
#define PRIVATE	25
    {"private",		TRUE,	0},
};

static png_color palette[256];

/*************************************************************************
 *
 * Color-name handling
 *
 ************************************************************************/

static color_item *cname_hashbuckets[COLOR_HASH_MODULUS];
static int cname_initialized;
static int write_transform_options;

static int hash_by_cname(color_item *cp)
/* hash by color's RGB value */
{
    unsigned int h = 0;
    char *p;

    for (p = cp->name; *p; p++)
	h = COLOR_HASH_MODULUS * h + *p;
    return(h % COLOR_HASH_MODULUS);
}

static color_item *find_by_cname(char *name)
/* find a character name */
{
   color_item sc, *hp;

   sc.name = name;

    for (hp = cname_hashbuckets[hash_by_cname(&sc)]; hp; hp = hp->next)
	if (strcmp(hp->name, name) == 0)
	    return(hp);

    return((color_item *)NULL);
}

/*************************************************************************
 *
 * Token-parsing code
 *
 ************************************************************************/

static char token_buffer[BUFSIZ];
static int token_class;
#define STRING_TOKEN	1
#define PUNCT_TOKEN	2
#define WORD_TOKEN	3
static bool pushed;

static void escapes(cp, tp)
/* process standard C-style escape sequences in a string */
const char	*cp;	/* source string with escapes */
char		*tp;	/* target buffer for digested string */
{
    while (*cp)
    {
	int	cval = 0;

	if (*cp == '\\' && strchr("0123456789xX", cp[1]))
	{
	    char *dp;
	    const char *hex = "00112233445566778899aAbBcCdDeEfF";
	    int dcount = 0;

	    if (*++cp == 'x' || *cp == 'X')
		for (++cp; (dp = strchr(hex, *cp)) && (dcount++ < 2); cp++)
		    cval = (cval * 16) + (dp - hex) / 2;
	    else if (*cp == '0')
		while (strchr("01234567",*cp) != (char*)NULL && (dcount++ < 3))
		    cval = (cval * 8) + (*cp++ - '0');
	    else
		while ((strchr("0123456789",*cp)!=(char*)NULL)&&(dcount++ < 3))
		    cval = (cval * 10) + (*cp++ - '0');
	}
	else if (*cp == '\\')		/* C-style character escapes */
	{
	    switch (*++cp)
	    {
	    case '\\': cval = '\\'; break;
	    case 'n': cval = '\n'; break;
	    case 't': cval = '\t'; break;
	    case 'b': cval = '\b'; break;
	    case 'r': cval = '\r'; break;
	    default: cval = *cp;
	    }
	    cp++;
	}
	else
	    cval = *cp++;
	*tp++ = cval;
    }
    *tp = '\0';
}

static int get_token(void)
/* grab a token from yyin */
{
    char	w, c, *tp = token_buffer;

    if (pushed)
    {
	pushed = FALSE;
	if (verbose > 1)
	    fprintf(stderr, "saved token: %s\n", token_buffer);
	return(TRUE);
    }

    /*
     * Skip leading whitespace.
     *
     * Treat commas, colons, and semicolons as whitespace -- this is
     * a mildly sleazy way to ensure that the compiler has
     * *really* tolerant syntax...in case we ever decide to change
     * this, there are comments reading "comma" at every point
     * where we might actually want same.
     */
    for (;;)
    {
	w = fgetc(yyin);
	if (w == '\n')
	    linenum++;
	if (feof(yyin))
	    return(FALSE);
	else if (isspace(w) || w == ',' || w == ';' || w == ':')
	    continue;
	else if (w == '#')		/* comment */
	{
	    for (;;)
	    {
		w = fgetc(yyin);
		if (feof(yyin))
		    return(FALSE);
		if (w == '\n')
		{
		    ungetc(w, yyin);
		    break;
		}
	    }
	}
	else				/* non-space character */
	{
	    *tp++ = w;
	    break;
	}
    }

    /* accumulate token */
    if (w == '\'' || w == '"')
    {
	int literal = FALSE;

	tp = token_buffer;
	for (;;)
	{
	    c = fgetc(yyin);
	    if (feof(yyin))
		return(FALSE);
	    else if (c == '\\' && !literal)
	    {
		literal = TRUE;
		continue;
	    }
	    else if (c == w && !literal)
		break;
	    else if (c == '\n' && !literal)
		fatal("runaway string");
	    else if (tp >= token_buffer + sizeof(token_buffer) - 1)
		fatal("string token too long");
	    else
	    {
		if (literal)
		{
		    *tp++ = '\\';
		    literal = FALSE;
		}
		*tp++ = c;
	    }
	}
	*tp = '\0';
	escapes(token_buffer, token_buffer);
	token_class = STRING_TOKEN;
    }
    else if (ispunct(w))
    {
	*tp = '\0';
	token_class = PUNCT_TOKEN;
    }
    else
    {
	for (;;)
	{
	    c = fgetc(yyin);
	    if (feof(yyin))
		return(FALSE);
	    else if (isspace(c))
	    {
		if (c == '\n')
		    linenum++;
		break;
	    }
	    else if (ispunct(c) && c != '.')
	    {
		ungetc(c, yyin);
		break;
	    }
	    else if (tp >= token_buffer + sizeof(token_buffer))
		fatal("token too long");
	    else
		*tp++ = c;
	}
	*tp = '\0';
	token_class = WORD_TOKEN;
    }

    if (verbose > 1)
	fprintf(stderr, "token: %s\n", token_buffer);
    return(TRUE);
}

static int token_equals(const char *str)
/* does the currently fetched token equal a specified string? */
{
    return !strcmp(str, token_buffer);
}

static int get_inner_token(void)
/* get a token within a chunk specification */
{
    if (!get_token())
	fatal("unexpected EOF");
    return(!token_equals("}"));	/* do we see end delimiter? */
}

static void push_token(void)
/* push back a token; must always be followed immediately by get_token */
{
    if (verbose > 1)
	fprintf(stderr, "pushing token: %s\n", token_buffer);
    pushed = TRUE;
}

static void require_or_die(const char *str)
/* croak if the next token doesn't match what we expect */
{
    if (!get_token())
	fatal("unexpected EOF");
    else if (!token_equals(str))
	fatal("unexpected token `%s' while waiting for %s", token_buffer, str);
}

static png_uint_32 long_numeric(bool token_ok)
/* validate current token as a PNG long (range 0..2^31-1) */
{
    unsigned long result;
    char *vp;

    if (!token_ok)
	fatal("EOF while expecting long-integer constant");
    result = strtoul(token_buffer, &vp, 0);
    if (*vp || result >= PNG_MAX_LONG)
	fatal("invalid or out of range long constant `%s'", token_buffer);
    return(result);
}

static png_int_32 slong_numeric(bool token_ok)
/* validate current token as a signed PNG long (range 0..2^31-1) */
{
    long result;
    char *vp;

    if (!token_ok)
	fatal("EOF while expecting signed long-integer constant");
    result = strtol(token_buffer, &vp, 0);
    if (*vp || result >= PNG_MAX_LONG || result <= -PNG_MAX_LONG)
	fatal("invalid or out of range long constant `%s'", token_buffer);
    return(result);
}

static png_uint_16 short_numeric(bool token_ok)
/* validate current token as a PNG long (range 0..2^16-1) */
{
    unsigned long result;
    char *vp;

    if (!token_ok)
	fatal("EOF while expecting short-integer constant");
    result = strtoul(token_buffer, &vp, 0);
    if (*vp || result == 65536)
	fatal("invalid or out of range short constant `%s'", token_buffer);
    return(result);
}

static png_byte byte_numeric(bool token_ok)
/* validate current token as a byte */
{
    unsigned long result;
    char *vp;

    if (!token_ok)
	fatal("EOF while expecting byte constant");
    result = strtoul(token_buffer, &vp, 0);
    if (*vp || result > 255)
	fatal("invalid or out of range byte constant `%s'", token_buffer);
    return(result);
}

static double double_numeric(bool token_ok)
/* validate current token as a double-precision value */
{
    double result;
    char *vp;

    if (!token_ok)
	fatal("EOF while expecting double-precision constant");
    result = strtod(token_buffer, &vp);
    if (*vp || result < 0)
	fatal("invalid or out of range double-precision constant `%s'", token_buffer);
    return(result);
}

static int string_validate(bool token_ok, char *stash)
/* validate current token as a string */
{
    if (!token_ok)
	fatal("EOF while expecting string constant");
    /* else */
    {
	int	len = strlen(token_buffer);

	if (len > PNG_STRING_MAX_LENGTH)
	    fatal("string token is too long");
	strncpy(stash, token_buffer, PNG_STRING_MAX_LENGTH);
	return(len);
    }
}

static int keyword_validate(bool token_ok, char *stash)
/* validate current token as a PNG keyword */
{
    if (!token_ok)
	fatal("EOF while expecting PNG keyword");
    /* else */
    {
	int	len = strlen(token_buffer);
	unsigned char	*cp;

	if (len > PNG_KEYWORD_MAX_LENGTH)
	    fatal("keyword token is too long");
	strncpy(stash, token_buffer, PNG_KEYWORD_MAX_LENGTH);
	if (isspace(stash[0]) || isspace(stash[len-1]))
	    fatal("keywords may not contain leading or trailing spaces");
	for (cp = (unsigned char *)stash; *cp; cp++)
	    if (*cp < 32 || (*cp > 126 && *cp < 161))
		fatal("keywords must contain Latin-1 characters only");
	    else if (isspace(cp[0]) && isspace(cp[1]))
		fatal("keywords may not contain consecutive spaces");
	return(len);
    }
}

static void collect_data(int *pnbytes, char **pbytes)
/* collect data in either bitmap format */
{
    /*
     * A data segment consists of a byte stream. 
     * There are presently three formats:
     *
     * string:
     *    An ASCII string, doublequote-delimited.
     *
     * base64: 
     *   One character per byte; values are as per RFC2045 base64:
     * 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/
     *
     * hex: 
     *   Two hex digits per byte.
     *
     * P1:
     *   pbm format P1 (see pbm(5)).
     *
     * P3:
     *   ppm format P3 (see ppm(5)).
     *
     * In either format, whitespace is ignored.
     */
    char *bytes = xalloc(MEMORY_QUANTUM);
    int quanta = 1;
    int	nbytes = 0;
    int ocount = 0;
    int c, maxval = 0;
#define BASE64_FMT	0
#define HEX_FMT		1
#define P1_FMT		2
#define P3_FMT		3
    int fmt = 0;

    if (!get_inner_token())
	fatal("missing format type in data segment");
    else if (token_class == STRING_TOKEN)
    {
	*pnbytes = 0;
	*pbytes = NULL;
	do {
	    int	seglen = strlen(token_buffer);

	    *pbytes = xrealloc(*pbytes, *pnbytes + seglen);
	    memcpy(*pbytes + *pnbytes, token_buffer, seglen);
	    *pnbytes += seglen;	    
	} while
	      (get_inner_token() && token_class == STRING_TOKEN);
	push_token();
	return;
    }
    else if (token_equals("base64"))
	fmt = BASE64_FMT;
    else if (token_equals("hex"))
	fmt = HEX_FMT;
    else if (token_equals("P1"))
    {
	int width = short_numeric(get_token());
	int height = short_numeric(get_token());

	if (width != info_ptr->width && height != info_ptr->height)
	    fatal("pbm image dimensions don't mastch IHDR");
	fmt = P1_FMT;
    }
    else if (token_equals("P3"))
    {
	int width = short_numeric(get_token());
	int height = short_numeric(get_token());

	maxval = short_numeric(get_token());

	if (width != info_ptr->width && height != info_ptr->height)
	    fatal("ppm image dimensions don't match IHDR");
	fmt = P3_FMT;
    }
    else
	fatal("unknown data format");

    while ((c = fgetc(yyin)))
    {
	if (feof(yyin))
	    fatal("unexpected EOF in data segment");
	else if (c == ';')
	    break;
	else if (c == '}')
	{
	    ungetc('}', yyin);
	    break;
	}
	else if (c == '#')	/* handle comments */
	{
	    while ((c = fgetc(yyin)))
		if (feof(yyin) || c == '\n')
		    break;
	    if (c == '\n')
		linenum++;
	    continue;
	}
	else if (isspace(c))	/* skip whitespace */
	{
	    if (c == '\n')
		linenum++;
	    continue;
	}
	else 
        {
	    unsigned char	value = 0;

	    if (nbytes >= quanta * MEMORY_QUANTUM)
		bytes = xrealloc(bytes, MEMORY_QUANTUM * ++quanta);

	    switch(fmt)
	    {
	    case BASE64_FMT:
		if (!isalpha(c) && !isdigit(c))
		    fatal("bad character %02x in data block", c);
		else if (isdigit(c))
		    value = c - '0';
		else if (isupper(c))
		    value = (c - 'A') + 10;
		else if (islower(c))
		    value = (c - 'a') + 36;
		else if (c == '+')
		    value = 62;
		else /* if (c == '/') */
		    value = 63;
		bytes[nbytes++] = value;
		break;

	    case HEX_FMT:
		if (!isxdigit(c))
		    fatal("bad hex character %02x in data block", c);
		else if (isdigit(c))
		    value = c - '0';
		else if (isupper(c))
		    value = (c - 'A') + 10;
		else 
		    value = (c - 'a') + 10;
		if (ocount++ % 2)
		    bytes[nbytes++] |= value;
		else
		    bytes[nbytes] = value * 16;
		break;

	    case P1_FMT:
		if (c == '0')
		    bytes[nbytes++] = 0;
		else if (c == '1')
		    bytes[nbytes++] = 1;
		else
		    fatal("bad pbm character %02x in data block", c);
		break;

	    case P3_FMT:
		c = short_numeric(get_token());

		if (c > maxval)
		    fatal("channel value out of range in pbm block");

		/*
		 * Channel order in PBM is R, then G, then B, same as PNG;
		 * so a straight copy in the order we see them will work.
		 */
		bytes[nbytes++] = c;
		break;
	    }
	}
    }
#undef BASE64_FMT
#undef HEX_FMT
#undef P1_FMT
#undef P3_FMT

    *pnbytes = nbytes;
    *pbytes = bytes;
}

/*************************************************************************
 *
 * The compiler itself
 *
 ************************************************************************/

static void compile_IHDR(void)
/* parse IHDR specification, set corresponding bits in info_ptr */
{
    int d = 0;

    /* read IHDR data */
    info_ptr->bit_depth = 8;
    info_ptr->color_type = PNG_COLOR_TYPE_GRAY;
    info_ptr->interlace_type = PNG_INTERLACE_NONE;
    while (get_inner_token())
	if (token_equals("height"))
	    info_ptr->height = long_numeric(get_token());
	else if (token_equals("width"))
	    info_ptr->width = long_numeric(get_token());
	else if (token_equals("bitdepth"))
	    d = byte_numeric(get_token());
        else if (token_equals("using"))
	    continue;			/* `uses' is just syntactic sugar */
        else if (token_equals("grayscale"))
	    continue;			/* so is grayscale */
        else if (token_equals("palette"))
	    info_ptr->color_type |= PNG_COLOR_MASK_PALETTE;
        else if (token_equals("color"))
	    info_ptr->color_type |= PNG_COLOR_MASK_COLOR;
        else if (token_equals("alpha"))
	    info_ptr->color_type |= PNG_COLOR_MASK_ALPHA;
        else if (token_equals("with"))
	    continue;			/* `with' is just syntactic sugar */
        else if (token_equals("interlace"))
	    info_ptr->interlace_type = PNG_INTERLACE_ADAM7;
	else
	    fatal("bad token `%s' in IHDR specification", token_buffer);

    /* IHDR sanity checks */
    if (!info_ptr->height)
	fatal("image height is zero or nonexistent");
    else if (!info_ptr->width)
	fatal("image width is zero or nonexistent");
    else if (d != 1 && d != 2 && d != 4 && d != 8 && d != 16)
	fatal("illegal bit depth %d in IHDR", d);
    else if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
    {
	if (d > 8)
	    fatal("bit depth of paletted images must be 1, 2, 4, or 8");
    }
    else if ((info_ptr->color_type != PNG_COLOR_TYPE_GRAY) && d!=8 && d!=16)
	fatal("bit depth of RGB- and alpha-using images must be 8 or 16");
    info_ptr->bit_depth = d;
}

static void compile_PLTE(void)
/* parse PLTE specification, set corresponding bits in info_ptr */
{
    int ncolors;

    initialize_hash(hash_by_cname, cname_hashbuckets, &cname_initialized);

    memset(palette, '\0', sizeof(palette));
    ncolors = 0;

    while (get_inner_token())
    {
	if (ncolors >= 256)
	    fatal("too many palette entries in PLTE specification");
	if (token_class == STRING_TOKEN)
	{
	    color_item *cp = find_by_cname(token_buffer);

	    if (!cp)
		fatal("unknown color name `%s' in PLTE", token_buffer);
	    else
	    {
		palette[ncolors].red = cp->r;
		palette[ncolors].green = cp->g;
		palette[ncolors].blue = cp->b;
		ncolors++;
	    }
	}
	else if (token_equals("("))
	{
	    palette[ncolors].red = byte_numeric(get_token());
	    /* comma */
	    palette[ncolors].green = byte_numeric(get_token());
	    /* comma */
	    palette[ncolors].blue = byte_numeric(get_token());
	    require_or_die(")");
	    ncolors++;
	}
	else
	    fatal("bad token %s in PLTE", token_buffer);
    }

    /* register the accumulated palette entries into the info structure */
    png_set_PLTE(png_ptr, info_ptr, palette, ncolors);
}

static void compile_IDAT(void)
/* parse IDAT specification and emit corresponding bits */
{
    int		nbits;
    char	*bits;
#ifdef PNG_INFO_IMAGE_SUPPORTED
    png_unknown_chunk	chunk;
#endif /* PNG_INFO_IMAGE_SUPPORTED */

    /*
     * Collect raw hex data and write it out as a chunk.
     */
    collect_data(&nbits, &bits);
    require_or_die("}");
#ifndef PNG_INFO_IMAGE_SUPPORTED
    png_write_chunk(png_ptr, "IDAT", bits, nbits);
    free(bits);
#else
    strcpy(chunk.name, "IDAT");
    chunk.data = bits;
    chunk.size = nbits;
    png_set_unknown_chunks(png_ptr, info_ptr, &chunk, 1);
    png_free(png_ptr, bits);
#endif /* PNG_INFO_IMAGE_SUPPORTED */
}

static void compile_cHRM(void)
/* parse cHRM specification, set corresponding bits in info_ptr */
{
    char	cmask = 0;
    float	wx = 0, wy = 0, rx = 0, ry = 0, gx = 0, gy = 0, bx = 0, by = 0;

    while (get_inner_token())
    {
	if (token_equals("white"))
	{
	    require_or_die("(");
	    wx = double_numeric(get_inner_token());
	    /* comma */
	    wy = double_numeric(get_inner_token());
	    require_or_die(")");
	    cmask |= 0x01;
	}
	else if (token_equals("red"))
	{
	    require_or_die("(");
	    rx = double_numeric(get_inner_token());
	    /* comma */
	    ry = double_numeric(get_inner_token());
	    require_or_die(")");
	    cmask |= 0x02;
	}
	else if (token_equals("green"))
	{
	    require_or_die("(");
	    gx = double_numeric(get_inner_token());
	    /* comma */
	    gy = double_numeric(get_inner_token());
	    require_or_die(")");
	    cmask |= 0x04;
	}
	else if (token_equals("blue"))
	{
	    require_or_die("(");
	    bx = double_numeric(get_inner_token());
	    /* comma */
	    by = double_numeric(get_inner_token());
	    require_or_die(")");
	    cmask |= 0x08;
	}
	else
	    fatal("invalid color `%s' name in cHRM specification",token_buffer);

    }

    if (cmask != 0x0f)
	fatal("cHRM specification is not complete");
    else
    {
#ifdef PNG_FLOATING_POINT_SUPPORTED
	png_set_cHRM(png_ptr, info_ptr,
		     wx, wy, rx, ry, gx, gy, bx, by);
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
	png_set_cHRM_fixed(png_ptr, info_ptr,
			   FLOAT_TO_FIXED(wx),
			   FLOAT_TO_FIXED(wy),
			   FLOAT_TO_FIXED(rx),
			   FLOAT_TO_FIXED(ry),
			   FLOAT_TO_FIXED(gx),
			   FLOAT_TO_FIXED(gy),
			   FLOAT_TO_FIXED(bx),
			   FLOAT_TO_FIXED(by));
#endif
#endif
    }
}

static void compile_gAMA(void)
/* compile and emit an gAMA chunk */
{
    double gamma = double_numeric(get_token());

#ifdef PNG_FLOATING_POINT_SUPPORTED
    png_set_gAMA(png_ptr, info_ptr, gamma);
#endif
#ifdef PNG_FIXED_POINT_SUPPORTED
    png_set_gAMA_fixed(png_ptr, info_ptr, FLOAT_TO_FIXED(gamma));
#endif
    if (!get_token() || !token_equals("}"))
	fatal("bad token `%s' in gAMA specification", token_buffer);
}

static void compile_iCCP(void)
/* compile and emit an iCCP chunk */
{
    int nname = 0, data_len = 0;
    char name[PNG_KEYWORD_MAX_LENGTH+1], *data;

    while (get_inner_token())
	if (token_equals("name"))
	    nname = keyword_validate(get_token(), name);
	else if (token_equals("profile"))
	    collect_data(&data_len, &data);

    if (!nname || !data_len)
	fatal("incomplete iCCP specification");

    png_set_iCCP(png_ptr, info_ptr, name, PNG_TEXT_COMPRESSION_NONE, 
		 data, data_len);
    free(data);
}

static void compile_sBIT(void)
/* compile an sBIT chunk, set corresponding bits in info_ptr */
{
    png_color_8	sigbits;
    bool color = (info_ptr->color_type & (PNG_COLOR_MASK_PALETTE | PNG_COLOR_MASK_COLOR));
    int sample_depth = ((info_ptr->color_type & PNG_COLOR_MASK_PALETTE) ? 8 : info_ptr->bit_depth);

    while (get_inner_token())
	if (token_equals("red"))
	{
	    if (!color)
		fatal("No color channels in this image type");
	    sigbits.red = byte_numeric(get_token());
	    if (sigbits.red > sample_depth)
		fatal("red sample depth out of range");
	}
	else if (token_equals("green"))
	{
	    if (!color)
		fatal("No color channels in this image type");
	    sigbits.green = byte_numeric(get_token());
	    if (sigbits.green > sample_depth)
		fatal("red sample depth out of range");
	}
	else if (token_equals("blue"))
	{
	    if (!color)
		fatal("No color channels in this image type");
	    sigbits.blue = byte_numeric(get_token());
	    if (sigbits.blue > sample_depth)
		fatal("red sample depth out of range");
	}
	else if (token_equals("gray"))
	{
	    if (color)
		fatal("No gray channel in this image type");
	    sigbits.gray = byte_numeric(get_token());
	    if (sigbits.gray > sample_depth)
		fatal("gray sample depth out of range");
	}
	else if (token_equals("alpha"))
	{
	    if (info_ptr->color_type & PNG_COLOR_MASK_ALPHA)
		fatal("No alpha channel in this image type");
	    sigbits.alpha = byte_numeric(get_token());
	    if (sigbits.alpha > sample_depth)
		fatal("alpha sample depth out of range");
	}
	else 
	    fatal("invalid channel name `%s' in sBIT specification",
		  token_buffer);

    png_set_sBIT(png_ptr, info_ptr, &sigbits);
}

static void compile_bKGD(void)
/* compile a bKGD chunk, put data in info structure */
{
    png_color_16	bkgbits;

    while (get_inner_token())
	if (token_equals("red"))
	{
	    if (!(info_ptr->color_type & PNG_COLOR_MASK_COLOR))
		fatal("Can't use color background with this image type");
	    bkgbits.red = short_numeric(get_token());
	}
	else if (token_equals("green"))
	{
	    if (!(info_ptr->color_type & PNG_COLOR_MASK_COLOR))
		fatal("Can't use color background with this image type");
	    bkgbits.green = short_numeric(get_token());
	}
	else if (token_equals("blue"))
	{
	    if (!(info_ptr->color_type & PNG_COLOR_MASK_COLOR))
		fatal("Can't use color background with this image type");
	    bkgbits.blue = short_numeric(get_token());
	}
	else if (token_equals("gray"))
	{
	    if (info_ptr->color_type & (PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_PALETTE))
		fatal("Can't use color background with this image type");
	    bkgbits.gray = short_numeric(get_token());
	}
	else if (token_equals("index"))
	{
	    if (!(info_ptr->color_type & PNG_COLOR_MASK_PALETTE))
		fatal("Can't use index background with a non-palette image");
	    bkgbits.index = byte_numeric(get_token());
	}
	else 
	    fatal("invalid channel `%s' name in bKGD specification", 
		  token_buffer);

    png_set_bKGD(png_ptr, info_ptr, &bkgbits);
}

static void compile_hIST(void)
/* compile a hIST chunk, put data in info structure */
{
    png_uint_16	hist[256];
    int		nhist = 0;

    while (get_inner_token())
	/* comma */
	hist[nhist++] = short_numeric(TRUE);

#ifndef MNG_INTERFACE
    if (nhist != info_ptr->num_palette)
	fatal("number of hIST values (%d) for palette doesn't match palette size (%d)", nhist, info_ptr->num_palette);
#else
    if (nhist != info_ptr->palette.size)
	fatal("number of hIST values (%d) for palette doesn't match palette size (%d)", nhist, info_ptr->palette.size);
#endif /* GUG */

    png_set_hIST(png_ptr, info_ptr, hist);
}

static void compile_tRNS(void)
/* compile a tRNS chunk, put data in info structure */
{
    png_byte	trans[256];
    int		ntrans = 0;
    png_color_16	tRNSbits;

    memset(&tRNSbits, '0', sizeof(tRNSbits));

    switch (info_ptr->color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
	require_or_die("gray");
	tRNSbits.gray = short_numeric(get_token());
	require_or_die("}");
	break;

    case PNG_COLOR_TYPE_PALETTE:
	while (get_inner_token())
	    /* comma */
	    trans[ntrans++] = byte_numeric(TRUE);
	break;

    case PNG_COLOR_TYPE_RGB:
	while (get_inner_token())
	    if (token_equals("red"))
		tRNSbits.red = short_numeric(get_token());
	    else if (token_equals("green"))
		tRNSbits.green = short_numeric(get_token());
	    else if (token_equals("blue"))
		tRNSbits.blue = short_numeric(get_token());
	    else 
		fatal("invalid channel name `%s' in tRNS specification", 
		      token_buffer);
	break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
    case PNG_COLOR_TYPE_GRAY_ALPHA:
	fatal("tRNS chunk not permitted with this image type");

    default:	/* should never happen */
	fatal("unknown color type");
    }

    png_set_tRNS(png_ptr, info_ptr, trans, ntrans, &tRNSbits);
}

static void compile_pHYs(void)
/* compile a pHYs chunk, put data in info structure */
{
    png_byte	unit = PNG_RESOLUTION_UNKNOWN;
    png_uint_32	res_x = 0, res_y = 0;

    while (get_inner_token())
	if (token_equals("xpixels"))
	    res_x = long_numeric(get_token());
	else if (token_equals("ypixels"))
	    res_y = long_numeric(get_token());
	else if (token_equals("per"))
	    continue;
	else if (token_equals("meter"))
	    unit = PNG_RESOLUTION_METER;
	else
	    fatal("invalid token `%s' in pHYs", token_buffer);

    if (!res_x || !res_y)
	fatal("illegal or missing resolutions in pHYs specification");

    png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit);
}

static void compile_sPLT(void)
/* compile sPLT chunk */
{
    char	keyword[PNG_KEYWORD_MAX_LENGTH+1];
    int		nentries = 0, nkeyword = 0;
    png_sPLT_t new_palette;
    png_sPLT_entry	entries[256];

    initialize_hash(hash_by_cname, cname_hashbuckets, &cname_initialized);

    new_palette.depth = 0;
    while (get_inner_token())
	if (token_equals("name"))
	    nkeyword = keyword_validate(get_token(), keyword);
        else if (token_equals("depth"))
	{
	    new_palette.depth = byte_numeric(get_token());
	    if (new_palette.depth != 8 && new_palette.depth != 16)
		fatal("invalid sample depth in sPLT");
	}
	else if (token_class == STRING_TOKEN)
	{
	    color_item *cp = find_by_cname(token_buffer);

	    if (!cp)
		fatal("unknown color name `%s' in PLTE", token_buffer);
	    else
	    {
		if (nentries >= 256)
		    fatal("too many palette entries in sPLT specification");
		palette[nentries].red = cp->r;
		palette[nentries].green = cp->g;
		palette[nentries].blue = cp->b;

		/* comma */
		entries[nentries].alpha = short_numeric(get_token());
		if (new_palette.depth == 8 && entries[nentries].alpha > 255)
		    fatal("alpha value too large for sample depth");
		/* comma */
		entries[nentries].frequency = short_numeric(get_token());
		nentries++;
	    }
	}
	else if (token_equals("("))
	{
	    if (nentries >= 256)
		fatal("too many palette entries in sPLT specification");
	    entries[nentries].red = short_numeric(get_token());
	    if (new_palette.depth == 8 && entries[nentries].red > 255)
		fatal("red value too large for sample depth");
	    /* comma */
	    entries[nentries].green = short_numeric(get_token());
	    if (new_palette.depth == 8 && entries[nentries].green > 255)
		fatal("green value too large for sample depth");
	    /* comma */
	    entries[nentries].blue = short_numeric(get_token());
	    if (new_palette.depth == 8 && entries[nentries].blue > 255)
		fatal("blue value too large for sample depth");
	    require_or_die(")");

	    /* comma */
	    entries[nentries].alpha = short_numeric(get_token());
	    if (new_palette.depth == 8 && entries[nentries].alpha > 255)
		fatal("alpha value too large for sample depth");
	    /* comma */
	    entries[nentries].frequency = short_numeric(get_token());
	    nentries++;
	}
	else
	    fatal("bad token `%s' in sPLT description", token_buffer);

    if (!nkeyword || !new_palette.depth)
	fatal("incomplete sPLT specification");

    new_palette.entries = entries;
    new_palette.nentries = nentries;
    new_palette.name = keyword;
    png_set_sPLT(png_ptr, info_ptr, &new_palette, 1);
}

static void compile_tEXt(void)
/* compile a text chunk; queue it up to be emitted later */
{
    char	keyword[PNG_KEYWORD_MAX_LENGTH+1];
    char	text[PNG_STRING_MAX_LENGTH+1];
    int		nkeyword = 0, ntext = 0;
    png_text	textblk;

    while (get_inner_token())
	if (token_equals("keyword"))
	    nkeyword = keyword_validate(get_token(), keyword);
	else if (token_equals("text"))
	    ntext = string_validate(get_token(), text);
	else
	    fatal("bad token `%s' in tEXt specification", token_buffer);

    if (!nkeyword || !ntext)
	fatal("keyword or text is missing in tEXt specification");

#ifdef PNG_iTXt_SUPPORTED
    textblk.lang = (char *)NULL;
#endif
    textblk.key = keyword;
    textblk.text = text;
    textblk.compression = PNG_TEXT_COMPRESSION_NONE;

    png_set_text(png_ptr, info_ptr, &textblk, 1);
}

static void compile_zTXt(void)
/* compile and emit a zTXt chunk */
{
    char	keyword[PNG_KEYWORD_MAX_LENGTH+1];
    char	text[PNG_STRING_MAX_LENGTH+1];
    int		nkeyword = 0, ntext = 0;
    png_text	textblk;

    while (get_inner_token())
	if (token_equals("keyword"))
	    nkeyword = keyword_validate(get_token(), keyword);
	else if (token_equals("text"))
	    ntext = string_validate(get_token(), text);
	else
	    fatal("bad token `%s' in zTXt specification", token_buffer);

    if (!nkeyword || !ntext)
	fatal("keyword or text is missing in zTXt specification");

#ifdef PNG_iTXt_SUPPORTED
    textblk.lang = (char *)NULL;
#endif
    textblk.key = keyword;
    textblk.text = text;
    textblk.compression = PNG_TEXT_COMPRESSION_zTXt;

    png_set_text(png_ptr, info_ptr, &textblk, 1);
}

static void compile_iTXt(void)
/* compile and emit an iTXt chunk */
{
    char	language[PNG_KEYWORD_MAX_LENGTH+1];
    char	keyword[PNG_KEYWORD_MAX_LENGTH+1]; 
    char	transkey[PNG_KEYWORD_MAX_LENGTH+1]; 
    char	text[PNG_STRING_MAX_LENGTH+1];
    int		nlanguage = 0, nkeyword = 0, ntranskey = 0, ntext = 0;
    int		compression = PNG_TEXT_COMPRESSION_NONE;
    png_text	textblk;

    compression = PNG_ITXT_COMPRESSION_NONE;
    while (get_inner_token())
	if (token_equals("language"))
	    nlanguage = keyword_validate(get_token(), language);
	else if (token_equals("keyword"))
	    nkeyword = keyword_validate(get_token(), keyword);
 	else if (token_equals("translated"))
	    ntranskey = keyword_validate(get_token(), transkey);
	else if (token_equals("text"))
	    ntext = string_validate(get_token(), text);
	else if (token_equals("compressed"))
	    compression = PNG_ITXT_COMPRESSION_zTXt;
	else
	    fatal("bad token `%s' in iTXt specification", token_buffer);

    if (!language || !keyword || !transkey || !text)
	fatal("keyword or text is missing");

    textblk.key = keyword;
#ifdef PNG_iTXt_SUPPORTED
    textblk.lang = language;
    textblk.lang_key = transkey;
#endif
    textblk.text = text;
    textblk.compression = compression;

    png_set_text(png_ptr, info_ptr, &textblk, 1);
}

static void compile_tIME(void)
/* compile a tIME chunk, put data in info structure */
{
    png_time stamp;
    int time_mask = 0;

    while (get_inner_token())
	if (token_equals("year"))
	{
	    stamp.year = short_numeric(get_token());
	    time_mask |= 0x01;
	}
	else if (token_equals("month"))
	{
	    stamp.month = byte_numeric(get_token());
	    if (stamp.month < 1 || stamp.month > 12)
		fatal("month value out of range");
	    time_mask |= 0x02;
	}
	else if (token_equals("day"))
	{
	    stamp.day = byte_numeric(get_token());
	    if (stamp.day < 1 || stamp.day > 31)
		fatal("day value out of range");
	    time_mask |= 0x04;
	}
	else if (token_equals("hour"))
	{
	    stamp.hour = byte_numeric(get_token());
	    if (stamp.hour > 23)
		fatal("hour value out of range");
	    time_mask |= 0x08;
	}
	else if (token_equals("minute"))
	{
	    stamp.minute = byte_numeric(get_token());
	    if (stamp.minute > 59)
		fatal("minute value out of range");
	    time_mask |= 0x10;
	}
	else if (token_equals("second"))
	{
	    stamp.second = byte_numeric(get_token());
	    if (stamp.second > 59)
		fatal("second value out of range");
	    time_mask |= 0x20;
	}
	else
	    fatal("bad token `%s' in tIME specification", token_buffer);

    if (time_mask != 0x3f)
	fatal("incomplete tIME specification");

    png_set_tIME(png_ptr, info_ptr, &stamp);
}

static void compile_oFFs(void)
/* parse oFFs specification and set corresponding info fields */
{
    png_byte	unit = PNG_OFFSET_PIXEL;	/* default to pixels */
    png_int_32	res_x = 0, res_y = 0;

    while (get_inner_token())
	if (token_equals("xoffset"))
	    res_x = slong_numeric(get_token());
	else if (token_equals("yoffset"))
	    res_y = slong_numeric(get_token());
	else if (token_equals("unit"))
	    continue;
	else if (token_equals("pixels"))
	    unit = PNG_OFFSET_PIXEL;
	else if (token_equals("micrometers"))
	    unit = PNG_OFFSET_MICROMETER;
	else
	    fatal("invalid token `%s' in oFFs specification", token_buffer);

    if (!res_x || !res_y)
	fatal("illegal or missing offsets in oFFs specification");

    png_set_oFFs(png_ptr, info_ptr, res_x, res_y, unit);
}

static void compile_pCAL(void)
/* parse pCAL specification and set corresponding info fields */
{
    char	name[PNG_KEYWORD_MAX_LENGTH+1];
    char	unit[PNG_STRING_MAX_LENGTH+1];
    char	strbuf[PNG_STRING_MAX_LENGTH+1];
    char	*params[MAX_PARAMS];
    int 	eqtype = 0, mask = 0, nname = 0, nunit = 0;
    int		nstrbuf = 0, nparams = 0, required = 0;
    png_int_32	x0 = 0, x1 = 0;

    while (get_inner_token())
	if (token_equals("name"))
	{
	    nname = keyword_validate(get_token(), name);
	    mask |= 0x01;
	}
	else if (token_equals("x0"))
	{
	    x0 = slong_numeric(get_token());
	    mask |= 0x02;
	}
	else if (token_equals("x1"))
	{
	    x1 = slong_numeric(get_token());
	    mask |= 0x04;
	}
	else if (token_equals("mapping"))
	    continue;
	else if (token_equals("linear"))
	{
	    eqtype = PNG_EQUATION_LINEAR;
	    mask |= 0x08;
	}
	else if (token_equals("euler"))
	{
	    eqtype = PNG_EQUATION_BASE_E;
	    mask |= 0x08;
	}
	else if (token_equals("exponential"))
	{
	    eqtype = PNG_EQUATION_ARBITRARY;
	    mask |= 0x08;
	}
	else if (token_equals("hyperbolic"))
	{
	    eqtype = PNG_EQUATION_HYPERBOLIC;
	    mask |= 0x08;
	}
	else if (token_equals("unit"))
	{
	    nunit = keyword_validate(get_token(), unit);
	    mask |= 0x10;
	}
        else if (token_equals("parameters"))
	{
	    nparams = 0;
	    while (get_inner_token())
		if (nparams >= MAX_PARAMS)
		    fatal("too many parameters in pCAL specification");
		else
		{
		    nstrbuf = string_validate(TRUE, strbuf);
		    params[nparams++] = xstrdup(strbuf);
		}
	    push_token();
	}
	else
	    fatal("invalid token `%s' in pCAL specification", token_buffer);

    /* validate the specification */
    if (!(mask != 0x01))
	fatal("incomplete pCAL specification: calibration name is missing");
    else if (!(mask != 0x01))
	fatal("incomplete pCAL specification: x0 is missing");
    else if (!(mask != 0x04))
	fatal("incomplete pCAL specification: x1 name is missing");
    else if (!(mask != 0x08))
	fatal("incomplete pCAL specification: equation type is missing");
    else if (!(mask != 0x10))
	fatal("incomplete pCAL specification: unit name is missing");

    switch (eqtype)
    {
    case PNG_EQUATION_LINEAR:
	required = 2;
	break;
    case PNG_EQUATION_BASE_E:
	required = 3;
	break;
    case PNG_EQUATION_ARBITRARY:
	required = 3;
	break;
    case PNG_EQUATION_HYPERBOLIC:
	required = 4;
	break;
    default:	/* should never happen! */
	fatal("unknown equation type in pCAL specification");
    }

    if (nparams != required)
	fatal("%d parameters is wrong for this equation type in pCAL",nparams);

    png_set_pCAL(png_ptr, info_ptr,
		 name, x0, x1, eqtype, nparams, unit, params);
}

static void compile_sCAL(void)
/* parse sCAL specification and emit corresponding bits */
{
    char	unit[PNG_STRING_MAX_LENGTH+1];
    double	width = 0, height = 0;
    int 	nunit = 0;
    png_byte	unitbyte = 0;
#if !defined(FLOATING_POINT_SUPPORTED) && defined(FIXED_POINT_SUPPORTED)
    char	width_s[BUFSIZ], height_s[BUFSIZ];
#endif

    while (get_inner_token())
	if (token_equals("unit"))
	{
	    nunit = string_validate(get_token(), unit);
	    if (token_equals("meter"))
		unitbyte = PNG_SCALE_METER;
	    else if (token_equals("radian"))
		unitbyte = PNG_SCALE_RADIAN;
	    else
		unitbyte = PNG_SCALE_UNKNOWN;
	}
	else if (token_equals("width"))
	{
	    height = double_numeric(get_token());
#if !defined(FLOATING_POINT_SUPPORTED) && defined(FIXED_POINT_SUPPORTED)
	    strcpy(width_s, token_buffer);
#endif
	}
	else if (token_equals("height"))
	{
	    width = double_numeric(get_token());
#if !defined(FLOATING_POINT_SUPPORTED) && defined(FIXED_POINT_SUPPORTED)
	    strcpy(height_s, token_buffer);
#endif
	}
	else
	    fatal("invalid token `%s' in pCAL specification", token_buffer);

    if (!nunit || !width || !height)
	fatal("incomplete sCAL specification");

#ifdef PNG_FLOATING_POINT_SUPPORTED
    png_set_sCAL(png_ptr, info_ptr, unitbyte, width, height);
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
    png_set_sCAL_s(png_ptr, info_ptr, unitbyte, width_s, height_s);
#endif
#endif
}

static void compile_gIFg(void)
/* parse gIFg specification and queue up the corresponding chunk */
{
    png_byte chunkdata[4];
    png_unknown_chunk chunk;

    memset(&chunk, '\0', sizeof(chunk));
    memset(chunkdata, '\0', sizeof(chunkdata));
    strcpy(chunk.name, "gIFg");
    chunk.data = chunkdata;
    chunk.size = 4;

    while (get_inner_token())
	if (token_equals("disposal"))
	    chunkdata[0] = byte_numeric(get_token());
	else if (token_equals("input"))
	    chunkdata[1] = byte_numeric(get_token());
	else if (token_equals("delay"))
	{
	    double delay = double_numeric(get_token());

	    png_save_uint_16(chunkdata+2, (int)(delay*100));
	}
	else
	    fatal("invalid token `%s' in gIFg specification", token_buffer);

    png_set_unknown_chunks(png_ptr, info_ptr, &chunk, 1);
}

static void compile_gIFx(void)
/* parse gIFx specification and queue up the corresponding chunk */
{
    png_byte chunkdata[PNG_STRING_MAX_LENGTH], buf[PNG_STRING_MAX_LENGTH];
    png_unknown_chunk chunk;

    memset(&chunk, '\0', sizeof(chunk));
    memset(chunkdata, '\0', sizeof(chunkdata));
    strcpy(chunk.name, "gIFx");
    chunk.data = chunkdata;

    while (get_inner_token())
	if (token_equals("identifier"))
	{
	    if (string_validate(get_token(), buf) != 8)
		fatal("application identifier has wrong length");
	    else
		memcpy(chunkdata, buf, 8);
	}
	else if (token_equals("code"))
	{
	    if (string_validate(get_token(), buf) != 3)
		fatal("authentication code has wrong length");
	    else
		memcpy(chunkdata + 8, buf, 3);
	}
	else if (token_equals("data"))
	{
	    int datalen;
	    char *data;

	    collect_data(&datalen, &data);
	    memcpy(chunkdata + 11, data, datalen);
	    free(data);
	}
	else
	    fatal("invalid token `%s' in gIFx specification", token_buffer);

    chunk.size = 11 + strlen(chunkdata + 11);

    png_set_unknown_chunks(png_ptr, info_ptr, &chunk, 1);
}

static void compile_IMAGE(void)
/* parse IMAGE specification and emit corresponding bits */
{
    int		i, nbytes, bytes_per_sample = 0, nsamples, input_width;
    char	*bytes;
    int		doublewidth = info_ptr->bit_depth == 16 ? 2 : 1;

    write_transform_options = 0;
    while (get_inner_token())
	if (token_equals("pixels"))
	    collect_data(&nbytes, &bytes);
	else if (token_equals("options"))
	{
	    if (token_equals("identity"))
		write_transform_options = PNG_TRANSFORM_IDENTITY;
	    else if (token_equals("packing"))
		write_transform_options |= PNG_TRANSFORM_PACKING;
	    else if (token_equals("packswap"))
		write_transform_options |= PNG_TRANSFORM_PACKSWAP;
	    else if (token_equals("invert_mono"))
		write_transform_options |= PNG_TRANSFORM_INVERT_MONO;
	    else if (token_equals("shift"))
		write_transform_options |= PNG_TRANSFORM_SHIFT;
	    else if (token_equals("bgr"))
		write_transform_options |= PNG_TRANSFORM_BGR;
	    else if (token_equals("swap_alpha"))
		write_transform_options |= PNG_TRANSFORM_SWAP_ALPHA;
	    else if (token_equals("invert_alpha"))
		write_transform_options |= PNG_TRANSFORM_INVERT_ALPHA;
	    else if (token_equals("swap_endian"))
		write_transform_options |= PNG_TRANSFORM_SWAP_ENDIAN;
	    else if (token_equals("strip_filler"))
		write_transform_options |= PNG_TRANSFORM_STRIP_FILLER;
	}
	else
	    fatal("invalid token `%s' in IMAGE specification", token_buffer);

    /* compute input sample size in bits */
    switch (info_ptr->color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
	bytes_per_sample = doublewidth;
	break;

    case PNG_COLOR_TYPE_PALETTE:
	bytes_per_sample = 1;
	break;

    case PNG_COLOR_TYPE_RGB:
	bytes_per_sample = 3 * doublewidth;
	break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
	bytes_per_sample = 4 * doublewidth;
	break;

    case PNG_COLOR_TYPE_GRAY_ALPHA:
	bytes_per_sample = 2 * doublewidth;
	break;

    default:	/* should never happen */
	fatal("unknown color type");
    }

    /*
     * Compute the actual size of the image in samples.
     */
    input_width = nbytes / info_ptr->height;
    if (info_ptr->bit_depth >= 8)
	nsamples = nbytes / bytes_per_sample;
    else
    {
	int samples_per_byte = (8 / info_ptr->bit_depth);

	nsamples = nbytes * samples_per_byte;

	/*
	 * Images of bit depth less than 8 will have excess data in the
	 * byte corresponding to the last pixel(s) in a row when the
	 * width is not a multiple of 8.  Correct for this.
	 */
	if (info_ptr->width % 8)
	{
	    int excess_samples_per_line = (info_ptr->width % samples_per_byte);

	    if (excess_samples_per_line)
		nsamples -= info_ptr->height * (samples_per_byte - excess_samples_per_line);
	}
    }
    if (nsamples != info_ptr->width * info_ptr->height)
	fatal("sample count (%d) doesn't match width*height (%d*%d) in IHDR",
	      nsamples, info_ptr->width, info_ptr->height);

#ifdef PNG_DEBUG
#if (PNG_DEBUG >= 6)
    /* dump the data as a check */
    {
	int	n;

	fprintf(stderr, "image data:\n");
	for (n = 0; n < nbytes; n++)
	{
	    fprintf(stderr, "%02x ", bytes[n] & 0xff);
	    if ((n+1) % input_width == 0)
		fputc('\n', stderr);
	}
    }
#endif
#endif

#ifndef PNG_INFO_IMAGE_SUPPORTED
    /* got the bits; now write them out */
    row_pointers = (png_byte **)xalloc(sizeof(char *) * info_ptr->height);
    for (i = 0; i < info_ptr->height; i++)
	row_pointers[i] = &bytes[i * input_width];
    png_write_image(png_ptr, row_pointers);
    free(bytes);
#else
    /* got the bits; attach them to the info structure */
    info_ptr->row_pointers = (png_byte **)xalloc(sizeof(char *) * info_ptr->height);
    for (i = 0; i < info_ptr->height; i++)
	info_ptr->row_pointers[i] = &bytes[i * input_width];
    info_ptr->valid |= PNG_INFO_IDAT;
#endif /* PNG_INFO_IMAGE_SUPPORTED */
}

static void compile_private(char *name)
/* compile a private chunk */
{
    int			nbytes;
    char		*bytes;
    png_unknown_chunk	chunk;

    if (strlen(name) != 4)
	fatal("wrong length for chunk name %s", token_buffer);
    else
	strcpy(chunk.name, name);

    require_or_die("{");
    collect_data(&nbytes, &bytes);
    require_or_die("}");

    chunk.data = bytes;
    chunk.size = nbytes;
    png_set_unknown_chunks(png_ptr, info_ptr, &chunk, 1);
    png_free(png_ptr, bytes);
}

int sngc(FILE *fin, char *name, FILE *fout)
/* compile SNG on fin to PNG on fout */
{
    int	prevchunk, errtype;
    char buf[BUFSIZ];

    yyin = fin;
    file = name;
    linenum = 1;

    if (fgets(buf, sizeof(buf), fin) == NULL)
    {
	fputs("sng: no data in file\n", stderr);
	exit(1);
    }
    else if (strncmp("#SNG", buf, 3))
    {
	fputs("sng: this is not an sng file\n", stderr);
	exit(1);
    }

    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also check that
     * the library version is compatible with the one used at compile time,
     * in case we are using dynamically linked libraries.  REQUIRED.
     */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
				      (void *)NULL, NULL, NULL);

    if (png_ptr == NULL)
	return(2);

    /* Allocate/initialize the image information data.  REQUIRED */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
	png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
	return(2);
    }

    /* if errtype is not 1, this was generated by fatal() */ 
    if ((errtype = setjmp(png_jmpbuf(png_ptr)))) {
	if (errtype == 1)
	    fprintf(stderr, "%s:%d: libpng croaked\n", file, linenum);
	free(png_ptr);
	free(info_ptr);
	return errtype;
    }

    /* set up the output control if you are using standard C streams */
    png_init_io(png_ptr, fout);

    /* keep all unknown chunks, we'll dump them later */
    png_set_keep_unknown_chunks(png_ptr, 2, NULL, 0);

    write_transform_options = PNG_TRANSFORM_IDENTITY;

    /* interpret the following chunk specifications */
    prevchunk = NONE;
    while (get_token())
    {
	chunkprops *pp;

	for (pp = properties; 
		 pp < properties + sizeof(properties)/sizeof(chunkprops);
		 pp++)
	    if (token_equals(pp->name))
		goto ok;
	fatal("unknown chunk type `%s'", token_buffer);

    ok:
	if (!get_token())
	    fatal("unexpected EOF");
	if (!token_equals("{") && (pp - properties) != PRIVATE)
	    fatal("missing chunk delimiter");
	if (!pp->multiple_ok && pp->count > 0)
	    fatal("illegal repeated chunk");

	switch (pp - properties)
	{
	case IHDR:
	    if (prevchunk != NONE)
		fatal("IHDR chunk must come first");
	    compile_IHDR();
	    break;

	case PLTE:
	    if (properties[IDAT].count)
		fatal("PLTE chunk must come before IDAT");
	    else if (properties[bKGD].count)
		fatal("PLTE chunk encountered after bKGD");
	    else if (properties[tRNS].count)
		fatal("PLTE chunk encountered after tRNS");
	    else if (!(info_ptr->color_type & PNG_COLOR_MASK_PALETTE))
		fatal("PLTE chunk specified for non-palette image type");
#ifndef PNG_INFO_IMAGE_SUPPORTED
	    png_write_info_before_PLTE(png_ptr, info_ptr);
#endif /* PNG_INFO_IMAGE_SUPPORTED */
	    compile_PLTE();
	    break;

	case IDAT:
	    if (properties[IMAGE].count)
		fatal("can't mix IDAT and IMAGE specs");
	    if (prevchunk != IDAT && pp->count)
		fatal("IDAT chunks must be contiguous");
	    /* force out the pre-IDAT portions */
#ifndef PNG_INFO_IMAGE_SUPPORTED
	    if (properties[IDAT].count == 0)
		png_write_info(png_ptr, info_ptr);
#endif /* PNG_INFO_IMAGE_SUPPORTED */
	    compile_IDAT();
	    break;

	case cHRM:
	    if (properties[PLTE].count || properties[IDAT].count)
		fatal("cHRM chunk must come before PLTE and IDAT");
	    compile_cHRM();
	    break;

	case gAMA:
	    if (properties[PLTE].count || properties[IDAT].count)
		fatal("gAMA chunk must come before PLTE and IDAT");
	    compile_gAMA();
	    break;

	case iCCP:
	    if (properties[PLTE].count || properties[IDAT].count)
		fatal("iCCP chunk must come before PLTE and IDAT");
	    compile_iCCP();
	    break;

	case sBIT:
	    if (properties[PLTE].count || properties[IDAT].count)
		fatal("sBIT chunk must come before PLTE and IDAT");
	    compile_sBIT();
	    break;

	case sRGB:
	    if (properties[PLTE].count || properties[IDAT].count)
		fatal("sRGB chunk must come before PLTE and IDAT");
	    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr,
				       byte_numeric(get_token()));
	    if (!get_token() || !token_equals("}"))
		fatal("bad token `%s' in sRGB specification", token_buffer);
	    break;

	case bKGD:
	    if (properties[IDAT].count)
		fatal("bKGD chunk must come between PLTE (if any) and IDAT");
	    compile_bKGD();
	    break;

	case hIST:
	    if (!properties[PLTE].count || properties[IDAT].count)
		fatal("hIST chunk must come between PLTE and IDAT");
	    compile_hIST();
	    break;

	case tRNS:
	    if (properties[IDAT].count)
		fatal("tRNS chunk must come between PLTE (if any) and IDAT");
	    compile_tRNS();
	    break;

	case pHYs:
	    if (properties[IDAT].count)
		fatal("pHYs chunk must come before IDAT");
	    compile_pHYs();
	    break;

	case sPLT:
	    if (properties[IDAT].count)
		fatal("sPLT chunk must come before IDAT");
	    compile_sPLT();
	    break;

	case tIME:
	    compile_tIME();
	    break;

	case iTXt:
	    compile_iTXt();
	    break;

	case tEXt:
	    compile_tEXt();
	    break;

	case zTXt:
	    compile_zTXt();
	    break;

	case oFFs:
	    if (properties[IDAT].count)
		fatal("oFFs chunk must come before IDAT");
	    compile_oFFs();
	    break;

	case pCAL:
	    if (properties[IDAT].count)
		fatal("pCAL chunk must come before IDAT");
	    compile_pCAL();
	    break;

	case sCAL:
	    if (properties[IDAT].count)
		fatal("sCAL chunk must come before IDAT");
	    compile_sCAL();
	    break;

	case gIFg:
	    compile_gIFg();
	    break;

	case gIFt:
	    fatal("gIFt chunks are not handled");
	    break;

	case gIFx:
	    compile_gIFx();
	    break;

	case fRAc:
	    fatal("fRAc chunk type is not defined yet");
	    break;

	case IMAGE:
	    if (properties[IDAT].count)
		fatal("can't mix IDAT and IMAGE specs");
	    /* force out the pre-IDAT portions */
#ifndef PNG_INFO_IMAGE_SUPPORTED
	    if (properties[IMAGE].count == 0)
		png_write_info(png_ptr, info_ptr);
#endif /* PNG_INFO_IMAGE_SUPPORTED */
	    compile_IMAGE();
	    properties[IDAT].count++;
	    break;

	case PRIVATE:
	    compile_private(token_buffer);
	    break;
	}

	if (verbose > 1)
	    fprintf(stderr, "%s specification processed\n", pp->name);
	prevchunk = (pp - properties);
	pp->count++;
    }

    /* end-of-file sanity checks */
    linenum = EOF;
    if (!properties[PLTE].count && (info_ptr->color_type & PNG_COLOR_MASK_PALETTE))
	fatal("palette property set, but no PLTE chunk found");
    if (!properties[IDAT].count)
	fatal("no image data");
    if (properties[iCCP].count && properties[sRGB].count)
	fatal("cannot have both iCCP and sRGB chunks (PNG spec 4.2.2.4)");

#ifndef PNG_INFO_IMAGE_SUPPORTED
    /* It is REQUIRED to call this to finish writing the rest of the file */
    png_write_end(png_ptr, info_ptr);
#else
    png_write_png(png_ptr, info_ptr, write_transform_options, NULL);
#endif /* PNG_INFO_IMAGE_SUPPORTED */

    /* if you malloced the palette, free it here */
    /* free(info_ptr->palette); */

    /* clean up after the write, and free any memory allocated */
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

    return(0);
}

/* sngc.c ends here */
