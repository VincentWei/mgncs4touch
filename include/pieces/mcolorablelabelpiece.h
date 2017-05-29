#ifndef _MGNCS_COLORABLELABELPIECE_H
#define _MGNCS_COLORABLELABELPIECE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mColorableLabelPiece 		mColorableLabelPiece;
typedef struct _mColorableLabelPieceClass 	mColorableLabelPieceClass;

#define mColorableLabelPieceClassHeader(clss, superCls) \
		mLabelPieceClassHeader(clss, superCls)

struct _mColorableLabelPieceClass
{
	mColorableLabelPieceClassHeader(mColorableLabelPiece, mLabelPiece)
};

#define mColorableLabelPieceHeader(clss) \
		mLabelPieceHeader(clss)			 \
		DWORD txtColor;

struct _mColorableLabelPiece
{
	mColorableLabelPieceHeader(mColorableLabelPiece)
};

MGNCS_EXPORT extern mColorableLabelPieceClass g_stmColorableLabelPieceCls;

enum mColorableLabelPieceProp {
	NCSP_CLP_PIECE_COLOR = USER_PIECE_PROP_BEGIN + 1,
	NCSP_CLP_PIECE_MAX,
};

#ifdef __cplusplus
}
#endif

#endif
