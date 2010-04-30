/* sng.h -- interface to the SNG compiler */

typedef struct color_item_t
{
    unsigned char r, g, b;
    char *name;
    struct color_item_t	*next;
}
color_item;

/* this modulus should be prime and close to the line count of rgb.txt */
#define COLOR_HASH_MODULUS	751

extern int sngc(FILE *fin, char *file, FILE *fout);
extern int sngd(FILE *fin, char *file, FILE *fout);

extern void fatal(const char *fmt, ... );
extern void *xalloc(unsigned long s);
extern void *xrealloc(void *p, unsigned long s);
extern char *xstrdup(char *s);

extern void initialize_hash(int hashfunc(color_item *),
			    color_item *hashbuckets[],
			    int *initflag);

extern int verbose;
extern int idat;
extern int sng_error;

extern int linenum;
extern char *file;
extern FILE *yyin;

extern png_struct *png_ptr;
extern png_info *info_ptr;

#define SUCCEED	0
#define FAIL	-1

#define TRUE	1
#define FALSE	0

/* RFC2045 encoding */
#define BASE64	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

/*
 * Maximum string size -- the size of an IDAT buffer minus the minimum overhead
 * of a string chunk (that is, the overhead of a minimal tEXt chunk).  
 * That overhead: four characters of chunk name, plus zero characters of 
 * keyword, plus one character of NUL separator.
 */
#define PNG_STRING_MAX_LENGTH	(PNG_ZBUF_SIZE - 5)

#define FLOAT_TO_FIXED(d)	((png_fixed_point)((d) * 100000))
#define FIXED_TO_FLOAT(n)	((float)((n) / 100000.0))

/* sng.h ends here */
