/* Setup projection function.
* ARGUMENTS: None.
* RETURNS: None.
*/

#include <stdio.h>
#include <string.h>

#include "anim.h"

/* Global transformation matrices */
MATR
  AK2_RndMatrWorld, /* World (object) transformation matrix */
  AK2_RndMatrView,  /* Camera view transform */
  AK2_RndMatrProj;  /* Projection transform */
/* Projection parameters */
DBL
  AK2_RndProjDist,  /* Near clip plane */
  AK2_RndFarClip,   /* Far clip plane */
  AK2_RndProjSize;  /* Project plane size */

UINT AK2_RndPrg = 0;

/* Materials array */
ak24MTL AK2_RndMaterials[AK2_MAX_MATERIALS];
INT AK2_RndNumOfMaterials;

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AK2_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (AK2_Anim.W >= AK2_Anim.H)
    ratio_x = (DBL)AK2_Anim.W / AK2_Anim.H;
  else
    ratio_y = (DBL)AK2_Anim.H / AK2_Anim.W;
  AK2_RndMatrProj = MatrFrustum(-ratio_x * AK2_RndProjSize / 2,
                                ratio_x * AK2_RndProjSize / 2,
                                -ratio_y * AK2_RndProjSize / 2,
                                ratio_y * AK2_RndProjSize / 2,
                                AK2_RndProjDist, AK2_RndFarClip);
} /* End of 'AK2_RndSetProj' function */


/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ak2OBJ *Obj;
 * RETURNS: None.
 */
VOID AK2_RndObjDraw( ak2OBJ *Obj )
{
  INT i;
  INT loc, mtl;
  MATR M, MSave;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    /* Build transform matrix */
    MSave = AK2_RndMatrWorld;
    AK2_RndMatrWorld = MatrMulMatr(AK2_RndMatrWorld, Obj->Prims[i].M);
    M = MatrMulMatr(AK2_RndMatrWorld,
      MatrMulMatr(AK2_RndMatrView, AK2_RndMatrProj));
    glLoadMatrixf(M.A[0]);
    /*
    glBegin(GL_LINES);
      glColor3d(1, 0, 0);
      glVertex3d(0, 0, 0);
      glVertex4d(1, 0, 0, 0);
      glColor3d(0, 1, 0);
      glVertex3d(0, 0, 0);
      glVertex4d(0, 1, 0, 0);
      glColor3d(0, 0, 1);
      glVertex3d(0, 0, 0);
      glVertex4d(0, 0, 1, 0);
    glEnd();
    */

    glUseProgram(AK2_RndPrg);

    mtl = Obj->Prims[i].MtlNo;
    if (mtl != -1)
    {
      if (AK2_RndMaterials[mtl].TexNo != 0)
      {
        glBindTexture(GL_TEXTURE_2D, AK2_RndMaterials[mtl].TexNo);
        if ((loc = glGetUniformLocation(AK2_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 1);
      }
      else
      {
        if ((loc = glGetUniformLocation(AK2_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 0);
      }
      if ((loc = glGetUniformLocation(AK2_RndPrg, "Ka")) != -1)
        glUniform3fv(loc, 1, &AK2_RndMaterials[mtl].Ka.X);
      if ((loc = glGetUniformLocation(AK2_RndPrg, "Kd")) != -1)
        glUniform3fv(loc, 1, &AK2_RndMaterials[mtl].Kd.X);
      if ((loc = glGetUniformLocation(AK2_RndPrg, "Ks")) != -1)
        glUniform3fv(loc, 1, &AK2_RndMaterials[mtl].Ks.X);
      if ((loc = glGetUniformLocation(AK2_RndPrg, "Ph")) != -1)
        glUniform1f(loc, AK2_RndMaterials[mtl].Ph);
      if ((loc = glGetUniformLocation(AK2_RndPrg, "Trans")) != -1)
        glUniform1f(loc, AK2_RndMaterials[mtl].Trans);
    }

    /* Setup global variables */
    if ((loc = glGetUniformLocation(AK2_RndPrg, "MatrWVP")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
    if ((loc = glGetUniformLocation(AK2_RndPrg, "MatrWorld")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, AK2_RndMatrWorld.A[0]);
    if ((loc = glGetUniformLocation(AK2_RndPrg, "MatrView")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, AK2_RndMatrView.A[0]);
    if ((loc = glGetUniformLocation(AK2_RndPrg, "MatrProj")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, AK2_RndMatrProj.A[0]);
    if ((loc = glGetUniformLocation(AK2_RndPrg, "Time")) != -1)
      glUniform1f(loc, AK2_Anim.Time);
    if ((loc = glGetUniformLocation(AK2_RndPrg, "PartNo")) != -1)
      glUniform1i(loc, i);

    /* Activete primitive vertex array */
    glBindVertexArray(Obj->Prims[i].VA);
    /* Activete primitive index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Obj->Prims[i].IBuf);
    /* Draw primitive */
    glDrawElements(GL_TRIANGLES, Obj->Prims[i].NumOfI, GL_UNSIGNED_INT, NULL);
    glUseProgram(0);
    AK2_RndMatrWorld = MSave;
  }
} /* End of 'AK2_RndObjDraw' function */

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ak2OBJ *Obj;
 * RETURNS: None.
 */
VOID AK2_RndObjFree( ak2OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    glBindVertexArray(Obj->Prims[i].VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Obj->Prims[i].VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Obj->Prims[i].VA);
    glDeleteBuffers(1, &Obj->Prims[i].IBuf);
  }
  free(Obj->Prims);
  memset(Obj, 0, sizeof(ak2OBJ));
} /* End of 'AK2_RndObjFree' function */

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT AK2_RndFindMaterial( CHAR *Name )
{
  INT i;

  for (i = 0; i < AK2_RndNumOfMaterials; i++)
    if (strcmp(Name, AK2_RndMaterials[i].Name) == 0)  /* #include <string.h> */
      return i;
  return -1;
} /* End of 'AK2_RndFindMaterial' function */

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID AK2_RndLoadMaterials( CHAR *FileName )
{
  INT i, NumOfMaterials /* , x, y */;
  UINT t;
  DWORD Sign;
  FILE *F;
  ak24MTL M;
  BYTE *Image;

  if ((F = fopen(FileName, "rb")) == NULL)
    return;
  /* Read and check file signature */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"GMT")
  {
    fclose(F);
    return;
  }

  /* Read all materials */
  fread(&NumOfMaterials, 4, 1, F);
  for (i = 0; i < NumOfMaterials; i++)
  {
    if (AK2_RndNumOfMaterials >= AK2_MAX_MATERIALS)
      break;

    /* Read illumination coefficients and texture parameters */
    fread(&M, sizeof(ak24MTL), 1, F);
    
    /* Read image */
    if (M.TexW != 0 && M.TexH != 0 && M.TexNo != 0)
    {
      /* Allocate memory for texture image */
      if ((Image = malloc(M.TexW * M.TexH * M.TexNo)) == NULL)
      {
        fclose(F);
        return;
      }
      fread(Image, M.TexNo, M.TexW * M.TexH, F);
      /* Flip image */
      /*
      for (y = 0; y < M.TexH / 2; y++)
        for (x = 0; x < M.TexW * M.TexNo; x++)
        {
          INT
            first = y * M.TexW * M.TexNo + x,
            last = (M.TexH - 1 - y) * M.TexW * M.TexNo + x;
          BYTE tmp = Image[first];

          Image[first] = Image[last];
          Image[last] = tmp;
        }
      */
      glGenTextures(1, &t);
      glBindTexture(GL_TEXTURE_2D, t);
      gluBuild2DMipmaps(GL_TEXTURE_2D, M.TexNo, M.TexW, M.TexH,
        M.TexNo == 3 ? GL_BGR_EXT : GL_BGRA, GL_UNSIGNED_BYTE, Image);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);
      M.TexNo = t;
      free(Image);
    }
    else
      M.TexNo = 0;
    /* Add material to animation and OpenGL */
    AK2_RndMaterials[AK2_RndNumOfMaterials++] = M;
  }
  fclose(F);
} /* End of 'AK2_RndLoadMaterials' function */

/* END OF 'RENDER.C' FILE */