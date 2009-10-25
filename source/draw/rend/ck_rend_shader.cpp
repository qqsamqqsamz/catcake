/*
    Copyright (c) 2007-2009 Takashi Kitao
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
    `  notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
    `  notice, this list of conditions and the following disclaimer in the
    `  documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products
    `  derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "pg_draw_all.h"


void pgRend_Shader::init(pgPrim* prim, pgID shd_id)
{
    if (shd_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgShd* shd = pgDrawMgr::getShader(shd_id);

    u32 rend_body_size = sizeof(RendBody) + shd->getUniformNum() * sizeof(r32);
    u32 rend_data_size = shd->getAttribNum() * sizeof(r32);

    pgRend::init(prim, rend_body_size, rend_data_size);

    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->shd = shd;
    rend_body->tex2 = NULL;
    rend_body->tex3 = NULL;
    rend_body->uni = reinterpret_cast<r32*>(rend_body + 1);
    rend_body->att = reinterpret_cast<r32*>(getRendData<u8>(0));
    rend_body->rend_data_size = rend_data_size;
}


void pgRend_Shader::init(pgPrim* prim, r32* rend_data, pgID shd_id)
{
    if (shd_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgShd* shd = pgDrawMgr::getShader(shd_id);

    u32 rend_body_size = sizeof(RendBody) + shd->getUniformNum() * sizeof(r32);
    u32 rend_data_size = shd->getAttribNum() * sizeof(r32);

    pgRend::init(prim, rend_body_size, rend_data_size, rend_data);

    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->shd = shd;
    rend_body->tex2 = NULL;
    rend_body->tex3 = NULL;
    rend_body->uni = reinterpret_cast<r32*>(rend_body + 1);
    rend_body->att = reinterpret_cast<r32*>(getRendData<u8>(0));
    rend_body->rend_data_size = rend_data_size;
}


pgID pgRend_Shader::getShaderID()
{
    RendBody* rend_body = getRendBody<RendBody>();

    return rend_body->shd->getID();
}


pgID pgRend_Shader::get2ndTextureID()
{
    RendBody* rend_body = getRendBody<RendBody>();
    pgTex* tex2 = rend_body->tex2;

    return tex2 ? tex2->getID() : pgID::ZERO;
}


void pgRend_Shader::set2ndTextureID(pgID tex_id)
{
    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->tex2 = (tex_id != pgID::ZERO) ? pgDrawMgr::getTexture(tex_id) : NULL;
}


pgID pgRend_Shader::get3rdTextureID()
{
    RendBody* rend_body = getRendBody<RendBody>();
    pgTex* tex3 = rend_body->tex3;

    return tex3 ? tex3->getID() : pgID::ZERO;
}


r32& pgRend_Shader::uniform(u8 uni_index)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index >= rend_body->shd->getUniformNum())
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return rend_body->uni[uni_index];
}


void pgRend_Shader::setUniformVec(u8 uni_index, const pgVec& vec)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index + 2 >= rend_body->shd->getUniformNum())
    {
        pgThrow(ExceptionInvalidArgument);
    }

    r32* uni = &rend_body->uni[uni_index];

    *(uni + 0) = vec.x;
    *(uni + 1) = vec.y;
    *(uni + 2) = vec.z;
}


void pgRend_Shader::setUniformCol3x255(u8 uni_index, pgCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index + 2 >= rend_body->shd->getUniformNum())
    {
        pgThrow(ExceptionInvalidArgument);
    }

    r32* uni = &rend_body->uni[uni_index];

    *(uni + 0) = col.r;
    *(uni + 1) = col.g;
    *(uni + 2) = col.b;
}


void pgRend_Shader::setUniformCol4x255(u8 uni_index, pgCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index + 3 >= rend_body->shd->getUniformNum())
    {
        pgThrow(ExceptionInvalidArgument);
    }

    r32* uni = &rend_body->uni[uni_index];

    *(uni + 0) = col.r;
    *(uni + 1) = col.g;
    *(uni + 2) = col.b;
    *(uni + 3) = col.a;
}


r32& pgRend_Shader::attrib(u16 data_index, u8 att_index)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index >= att_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return rend_body->att[att_num * data_index + att_index];
}


void pgRend_Shader::setAttribVec(u16 data_index, u8 att_index, const pgVec& vec)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index + 2 >= att_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    r32* att = &rend_body->att[att_num * data_index + att_index];

    *(att + 0) = vec.x;
    *(att + 1) = vec.y;
    *(att + 2) = vec.z;
}


void pgRend_Shader::setAttribCol3x255(u16 data_index, u8 att_index, pgCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index + 2 >= att_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    r32* att = &rend_body->att[att_num * data_index + att_index];

    *(att + 0) = col.r;
    *(att + 1) = col.g;
    *(att + 2) = col.b;
}


void pgRend_Shader::setAttribCol4x255(u16 data_index, u8 att_index, pgCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index + 3 >= att_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    r32* att = &rend_body->att[att_num * data_index + att_index];

    *(att + 0) = col.r;
    *(att + 1) = col.g;
    *(att + 2) = col.b;
    *(att + 3) = col.a;
}


void pgRend_Shader::set3rdTextureID(pgID tex_id)
{
    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->tex3 = (tex_id != pgID::ZERO) ? pgDrawMgr::getTexture(tex_id) : NULL;
}


pgID pgRend_Shader::getClassID()
{
    return pgID_("pgRend_Shader");
}


void pgRend_Shader::initData(void* data, u16 data_num) {}


void pgRend_Shader::render(const pgMat& view)
{
    pgPrim* prim = getPrim();

    if (!pgDrawMgr::isShaderAvailable())
    {
        renderCallPrimRenderWithDestroyingBuffer(prim, view);
        return;
    }

    u16 cur_data_num = prim->getCurDataNum();

    if (cur_data_num == 0)
    {
        return;
    }

    /*
        setup shader
    */
    RendBody* rend_body = getRendBody<RendBody>();
    pgShd* shd = rend_body->shd;

    if (shd->isValid())
    {
        renderSetShader(shd);
    }
    else
    {
        renderCallPrimRenderWithDestroyingBuffer(prim, view);
        return;
    }

    /*
        alloc buffer
    */
    pgTex* tex = renderGetPrimTextureN(prim);
    pgCol final_col = renderGetPrimFinalColor(prim);

    pgVec* pos_buf;
    pgCol* col_buf;
    r32* uv_buf;
    pgVec* normal_buf;

    renderAllocBuffer(&pos_buf, &col_buf, &uv_buf, &normal_buf, prim, false, (final_col != pgCol::FULL), (tex && renderIsTextureUVAdjustNeeded(tex)), false);

    /*
        setup color
    */
    renderSetColorPointer(0, NULL);

    if (col_buf)
    {
        renderCalcColorBuffer(col_buf, prim);
        renderSetAttribPointer_color(shd, sizeof(pgCol), reinterpret_cast<const u8*>(col_buf));
    }
    else
    {
        renderSetAttribPointer_color(shd, prim);
    }

    /*
        setup texture
    */
    renderSetTexture(tex, rend_body->tex2, rend_body->tex3, prim->isDrawFlag(pgDraw::FLAG_BILINEAR));
    renderSetTexCoordPointer(0, NULL);

    if (tex)
    {
        if (uv_buf)
        {
            renderCalcUVBuffer(uv_buf, prim);
            renderSetAttribPointer_texCoord(shd, sizeof(r32) * 2, uv_buf);
        }
        else
        {
            renderSetAttribPointer_texCoord(shd, prim);
        }
    }

    /*
        setup param
    */
    for (s32 i = 0; i < shd->getUniformNum(); i++)
    {
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, i), rend_body->uni[i]);
    }

    for (s32 i = 0; i < shd->getAttribNum(); i++)
    {
        u32 att_loc = renderGetShaderAttribLocation(shd, i);

        renderSetAttribPointer_r32(att_loc, 1, rend_body->rend_data_size, &rend_body->att[i]);
    }

    if (shd->getTextureNum() > 0)
    {
        renderSetUniform_s32(renderGetShaderTextureLocation(shd, 0), 0);
    }

    if (shd->getTextureNum() > 1)
    {
        renderSetUniform_s32(renderGetShaderTextureLocation(shd, 1), 1);
    }

    if (shd->getTextureNum() > 2)
    {
        renderSetUniform_s32(renderGetShaderTextureLocation(shd, 2), 2);
    }

    /*
        draw primitives
    */
    renderSetUniform_localToScreen(shd);

    renderSetVertexPointer(0, NULL);
    renderSetAttribPointer_vertex(shd, prim);

    renderDrawArrays(prim->getPrimMode(), 0, cur_data_num);

    /*
        disable attributes
    */
    renderDisableAttribPointers(shd);
}