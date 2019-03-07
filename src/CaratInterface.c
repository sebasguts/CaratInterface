/*
 * CaratInterface: 
 */

#include "src/compiled.h"          /* GAP headers */
#include "../carat/include/bravais.h"
#include "../carat/include/tools.h"
#include "../carat/include/matrix.h"
#include "../carat/include/getput.h"

Obj FuncBravais(Obj self, Obj matr_grp)
{
    matrix_TYP ** mat;
    int           len = LEN_LIST(matr_grp);
    mat = (matrix_TYP **)malloc(len * sizeof(matrix_TYP *));
    Obj current_mat = ELM0_LIST(matr_grp, 1);
    int curr_dim = LEN_LIST(current_mat);
    for (int i = 0; i < len; i++) {
        current_mat = ELM0_LIST(matr_grp, i + 1);
        mat[i] = init_mat(curr_dim, curr_dim, "");
        // mat[i]->array.N = (int**)malloc2dim(curr_dim,curr_dim,sizeof(int));
        for (int j = 1; j <= curr_dim; j++) {
            for (int k = 1; k <= curr_dim; k++) {
                Obj curr_elm = ELM0_LIST(ELM0_LIST(current_mat, j), k);
                mat[i]->array.SZ[j - 1][k - 1] = (int)Int_ObjInt(curr_elm);
            }
        }
        mat[i]->kgv = 1;
        mat[i]->flags.Integral = TRUE;
        mat[i]->flags.Symmetric = FALSE;
        mat[i]->flags.Diagonal = FALSE;
        mat[i]->flags.Scalar = FALSE;
        Check_mat(mat[i]);
    }
    int           x = 15;
    matrix_TYP ** return_value = formspace(mat, len, 0, &x);
    Obj           ret_list = NEW_PLIST(T_PLIST, len + 1);
    SET_LEN_PLIST(ret_list, len + 1);
    ASS_LIST(ret_list, 1, ObjInt_Int(x));
    for (int i = 0; i < x; i++) {
        Obj curr_ret_list = NEW_PLIST(T_PLIST, curr_dim);
        SET_LEN_PLIST(ret_list, curr_dim);
        for (int j = 0; j < curr_dim; j++) {
            Obj curr_col = NEW_PLIST(T_PLIST, curr_dim);
            SET_LEN_PLIST(curr_col, curr_dim);
            for (int k = 0; k < curr_dim; k++) {
                Obj curr_elm = ObjInt_Int(return_value[i]->array.SZ[j][k]);
                ASS_LIST(curr_col, k + 1, curr_elm);
                CHANGED_BAG(curr_col);
            }
            ASS_LIST(curr_ret_list, j + 1, curr_col);
            CHANGED_BAG(curr_ret_list);
        }
        ASS_LIST(ret_list, i + 2, curr_ret_list);
        CHANGED_BAG(ret_list);
    }
    for (int i = 0; i < len; i++) {
        free_mat(mat[i]);
    }
    for (int i = 0; i < x; i++) {
        free_mat(return_value[i]);
    }
    free(mat);
    return ret_list;
}

// Table of functions to export
static StructGVarFunc GVarFuncs [] = {
    GVAR_FUNC(Bravais, 1, "matr_grp"),

    { 0 } /* Finish with an empty entry */
};

/******************************************************************************
**
*F  InitKernel( <module> ) . . . . . . . . .  initialise kernel data structures
*/
static Int InitKernel( StructInitInfo *module )
{
    /* init filters and functions */
    InitHdlrFuncsFromTable( GVarFuncs );

    /* return success */
    return 0;
}

/******************************************************************************
**
*F  InitLibrary( <module> ) . . . . . . . .  initialise library data structures
*/
static Int InitLibrary( StructInitInfo *module )
{
    /* init filters and functions */
    InitGVarFuncsFromTable( GVarFuncs );

    /* return success */
    return 0;
}

/******************************************************************************
**
*F  Init__Dynamic() . . . . . . . . . . . . . . . . . . table of init functions
*/
static StructInitInfo module = {
    .type = MODULE_DYNAMIC,
    .name = "CaratInterface",
    .initKernel = InitKernel,
    .initLibrary = InitLibrary,
};

StructInitInfo *Init__Dynamic( void )
{
    return &module;
}
