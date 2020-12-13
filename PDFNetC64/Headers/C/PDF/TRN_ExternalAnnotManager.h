//---------------------------------------------------------------------------------------
// Copyright (c) 2001-2020 by PDFTron Systems Inc. All Rights Reserved.
// Consult legal.txt regarding legal and license information.
//---------------------------------------------------------------------------------------
// !Warning! This file is autogenerated, modify the .codegen file, not this one
// (any changes here will be wiped out during the autogen process)

#ifndef PDFTRON_H_CExternalAnnotManager
#define PDFTRON_H_CExternalAnnotManager

#ifdef __cplusplus
extern "C" {
#endif

#include <C/Common/TRN_Types.h>
#include <C/Common/TRN_Exception.h>


struct TRN_ExternalAnnotManager_tag;
typedef struct TRN_ExternalAnnotManager_tag* TRN_ExternalAnnotManager;


/* methods: */
TRN_API TRN_ExternalAnnotManagerMergeXFDF(TRN_ExternalAnnotManager self, TRN_UString xfdf_content);
TRN_API TRN_ExternalAnnotManagerUndo(TRN_ExternalAnnotManager self, TRN_UString* result);
TRN_API TRN_ExternalAnnotManagerGetLastXFDF(TRN_ExternalAnnotManager self, TRN_UString* result);
TRN_API TRN_ExternalAnnotManagerRedo(TRN_ExternalAnnotManager self, TRN_UString* result);
TRN_API TRN_ExternalAnnotManagerGetNextRedoInfo(TRN_ExternalAnnotManager self, TRN_UString* result);
TRN_API TRN_ExternalAnnotManagerGetNextUndoInfo(TRN_ExternalAnnotManager self, TRN_UString* result);
TRN_API TRN_ExternalAnnotManagerTakeSnapshot(TRN_ExternalAnnotManager self, TRN_UString payload, TRN_UString* result);

TRN_API TRN_ExternalAnnotManagerDestroy(TRN_ExternalAnnotManager* self);


#ifdef __cplusplus
} // extern C
#endif

#endif /* PDFTRON_H_CExternalAnnotManager */
