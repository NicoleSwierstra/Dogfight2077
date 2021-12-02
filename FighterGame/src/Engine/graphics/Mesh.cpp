#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <optick.h>

Mesh::Mesh(const std::string& filepath)
{
    if (filepath.find(".obj") != std::string::npos) {
        LoadObj(filepath.c_str());
    }
}

Mesh::~Mesh() {
    for (int i = 0; i < sm.size(); i++) {
        delete sm[i].vb;
        sm[i].verts.clear();
    }
}

Mesh::Mesh(std::vector<glm::vec3>& verts, std::vector<unsigned int>& indexes)
{
    if (verts.size() == 0 || indexes.size() == 0) {
        throw 1;
        return;
    }
    std::vector<vertex> vertices;
    std::vector<std::vector<glm::vec3>> norm_average;
    norm_average.resize(verts.size());
    for (int i = 0; i < indexes.size() - 1; i += 3) {
        int i0 = indexes[i],
            i1 = indexes[i + 1],
            i2 = indexes[i + 2];
        glm::vec3 
            p0 = verts[i0], 
            p1 = verts[i1], 
            p2 = verts[i2];
        glm::vec3 v = p1-p0, 
            w = p0-p2;
        glm::vec3 norm = glm::cross(v,w);
        vertices.push_back(vertex{p0, norm, glm::vec2(0.0f,0.0f)});
        vertices.push_back(vertex{p1, norm, glm::vec2(0.0f,0.0f)});
        vertices.push_back(vertex{p2, norm, glm::vec2(0.0f,0.0f)});
    }
    sm.push_back(submesh{new VertexBuffer(&vertices[0], vertices.size() * sizeof(vertex)), vertices});
}

void Mesh::LoadObj(const char* filepath)
{
    std::ifstream stream(filepath);
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> norm_read;
    std::vector<glm::vec2> texcords;
    bool usesMat = false;
    int subMeshNum = 0;
    sm.push_back(submesh{new VertexBuffer(nullptr, 0), std::vector<vertex>()});

    std::string line;
    while (std::getline(stream, line))
    {
        if (line.substr(0, 2) == "v ") {
            std::istringstream v(line.substr(2));
            float x, y, z;
            sscanf_s(line.c_str(), "v %f %f %f", &x, &y, &z);
            vertices.push_back(glm::vec3(x, y, z));
        }
        else if (line.substr(0, 2) == "vt") {
            std::istringstream v(line.substr(2));
            glm::vec2 tex;
            float x, y;
            sscanf_s(line.c_str(), "vt %f %f", &x, &y);
            texcords.push_back(glm::vec2(x, y));
        }
        else if (line.substr(0, 2) == "vn") {
            std::istringstream v(line.substr(2));
            glm::vec3 norm;
            float x, y, z;
            sscanf_s(line.c_str(), "vn %f %f %f", &x, &y, &z);
            norm_read.push_back(glm::vec3(x, z, y));
        }
        else if (line.substr(0, 2) == "vp");
        else if (line.substr(0, 6) == "usemtl") {
            if (!usesMat) {
                usesMat = true;
            }
            else {
                subMeshNum++;
                sm.push_back(submesh{ new VertexBuffer(nullptr, 0), std::vector<vertex>() });
            }
        }
        else if (line.substr(0, 2) == "f ") {
            unsigned int
                va, vb, vc, //to store mesh index
                vta, vtb, vtc, //to store texture index
                vna, vnb, vnc; //to store normals

            const char* chh = line.c_str();
            sscanf_s(chh, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &va, &vta, &vna, &vb, &vtb, &vnb, &vc, &vtc, &vnc);

            va--; vb--; vc--; 
            vta--; vtb--; vtc--; 
            vna--; vnb--; vnc--;

            sm[subMeshNum].verts.push_back(vertex{ vertices[va], norm_read[vna], texcords[vta] });
            sm[subMeshNum].verts.push_back(vertex{ vertices[vb], norm_read[vnb], texcords[vtb] });
            sm[subMeshNum].verts.push_back(vertex{ vertices[vc], norm_read[vnc], texcords[vtc] });
        }
    }
    sm[0].vb->Write(&sm[0].verts[0], sm[0].verts.size() * sizeof(vertex));
    for (int j = 1; j <= subMeshNum; j++) {
        sm[j].vb->Write(&sm[j].verts[0], sm[j].verts.size() * sizeof(vertex));
    }
}

Mesh* Mesh::AddShader(Shader* s, std::vector<uniformNode> un, int meshId, int type, bool wireframe){
    sh.push_back(shaderNode{ s, un, meshId, type, wireframe });
    return this;
}

Mesh* Mesh::AddShader(Shader* s, glm::mat4* proj, glm::mat4* view, std::vector<uniformNode> un, int meshId, int type, bool wireframe)
{
    un.push_back(uniformNode{ "proj", proj, UNIFORM_MAT4 });
    un.push_back(uniformNode{ "view", view, UNIFORM_MAT4 });
    sh.push_back(shaderNode{ s, un, meshId, type, wireframe });
    std::cout << sm[meshId].verts.size() << "\n";
    return this;
}

void Mesh::Optimise()
{
    for (submesh s : sm) {
        s.verts.clear();
    }
}

void Mesh::applyUniform(uniformNode* uniform, Shader* shader) {
    switch (uniform->type) {
    case UNIFORM_FLOAT:
        shader->SetFloatUniforms(uniform->name, *(float*)uniform->data);
        break;
    case UNIFORM_INT:
        shader->SetIntUniforms(uniform->name, *(int*)uniform->data);
        break;
    case UNIFORM_MAT4:
        shader->SetMat4Uniforms(uniform->name, *(glm::mat4*)uniform->data);
        break;
    case UNIFORM_VEC2:
        shader->SetV2Uniforms(uniform->name, *(glm::vec2*)uniform->data);
        break;
    case UNIFORM_VEC3:
        shader->SetV3Uniforms(uniform->name, *(glm::vec3*)uniform->data);
        break;
    case UNIFORM_VEC4:
        shader->SetV4Uniforms(uniform->name, *(glm::vec4*)uniform->data);
        break;
    }
}

void Mesh::applyUniforms(shaderNode* sn) {
    for (int i = 0; i < sn->uniforms.size(); i++) {
        applyUniform(&sn->uniforms[i], sn->shader);
    }
}

void Mesh::Render(){
    OPTICK_EVENT("RENDER");
    for (int i = 0; i < sh.size(); i++) {
        sh[i].shader->Bind();
        sm[sh[i].meshId].vb->Bind();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, 32, 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, 0, 32, (void*)12);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, 0, 32, (void*)24);
        applyUniforms(&sh[i]);
        glPolygonMode(GL_FRONT_AND_BACK, sh[i].wireframe ? GL_LINE : GL_FILL);
        {
            OPTICK_GPU_EVENT("DRAW ARRAYS");
            glDrawArrays(sh[i].type, 0, sm[sh[i].meshId].verts.size()); 
        }
    }
}

void Mesh::RenderMultiple(std::vector<glm::vec3>& positions, std::vector<uniformNode> uniforms, const char* uniform)
{
    OPTICK_EVENT("RENDER MULTI");
    for (int j = 0; j < positions.size(); j++) {
        for (int i = 0; i < sh.size(); i++) {
            sh[i].shader->Bind();
            sm[sh[i].meshId].vb->Bind();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, 0, 32, 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, 0, 32, (void*)12);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, 0, 32, (void*)24);
            applyUniforms(&sh[i]);
            glPolygonMode(GL_FRONT_AND_BACK, sh[i].wireframe ? GL_LINE : GL_FILL);
            glm::mat4 mat = glm::translate(glm::mat4(1.0f), positions[j]);
            sh[i].shader->SetMat4Uniforms(uniform, mat);
            applyUniform(&uniforms[j], sh[i].shader);
            {
                OPTICK_GPU_EVENT("DRAW ARRAYS");
                glDrawArrays(sh[i].type, 0, sm[sh[i].meshId].verts.size());
            }
        }
    }
}
