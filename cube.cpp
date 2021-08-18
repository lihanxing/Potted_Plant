#include "cube.h"
#include "resourcemanager.h"
#include <QtMath>
extern int PAINT_WIDTH;
extern int PAINT_HEIGHT;

QVector3D matMulvec(QMatrix4x4 matrix, QVector3D vector);
void computeTangentBasis(QVector<QVector3D> &positions, QVector<QVector2D> &uvs, QVector<QVector3D> &normals, QVector<QVector3D> &tangents, QVector<QVector3D> &bitangents);
GLboolean is_Near(GLfloat v1, GLfloat v2);
GLboolean getSimilarVertexIndex(QVector3D &in_positions, QVector<QVector3D> &out_positions, GLushort &result);
void indexVBO_TBN(
    QVector<QVector3D> &in_positions,
    QVector<QVector2D> &in_uvs,
    QVector<QVector3D> &in_normals,
    QVector<QVector3D> &in_tangents,
    QVector<QVector3D> &in_bitangents,

    QVector<GLushort> &out_indices,
    QVector<QVector3D> &out_positions,
    QVector<QVector2D> &out_uvs,
    QVector<QVector3D> &out_normals,
    QVector<QVector3D> &out_tangents,
    QVector<QVector3D> &out_bitangents
);


Cube::Cube(){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();


}

Cube::~Cube(){
  core->glDeleteBuffers(1, &posVBO);
  core->glDeleteBuffers(1, &uvVBO);
  core->glDeleteBuffers(1, &norVBO);
  core->glDeleteBuffers(1, &tanVBO);
  core->glDeleteBuffers(1, &bitVBO);
  core->glDeleteBuffers(1, &elementVBO);
}

/*
  0 1    4  3
  2         5
    5    2
  4 3    0  1

*/


//GLuint quadNum = 9;


bool Cube::init(QVector<QPointF> bez_points){
  /***********88*/
  indices.clear();
  indexed_bitangents.clear();
  indexed_normals.clear();
  indexed_positions.clear();
  indexed_tangents.clear();
  indexed_uvs.clear();
  /***********88*/

//    qDebug() << "bezier size: " << bez_points.size();
//    for(int i =0; i < bez_points.size(); ++i){
//      qDebug() << bez_points[i];
//    }

  QPointF deltaPos;
  QVector<float> angles;
  bool isLeft;
  for(int i = 1; i < bez_points.size(); ++i){//12个点能得到11个角度
    deltaPos = bez_points[i] - bez_points[0];
    if(deltaPos.x() > 0)
      angles.push_back(90.0f + qRadiansToDegrees(qAtan(deltaPos.y()/deltaPos.x())));
    else
      angles.push_back(-90.0f + qRadiansToDegrees(qAtan(deltaPos.y()/deltaPos.x())));
  }
  angles.push_front(0.0f);
  float delHTX = bez_points[bez_points.size()-1].x() - bez_points[0].x(); //这个是一条线首尾的x坐标差值
  if(delHTX < 0)
    isLeft = true;
  else
    isLeft = false;

  delHTX = qFabs(delHTX);
  float delHTY = qFabs(bez_points[bez_points.size()-1].y() - bez_points[0].y());//不管横着画，还是竖着画，都可以得到最大长度
//  qDebug() << "angles size: " << angles.size();
//  for(int i =0;i < angles.size(); ++i){
//    qDebug() << angles[i];
//  }

  QVector<QVector3D> positions;
  QVector<QVector2D> uvs;
  QVector<QVector3D> normals;

  GLuint quadNum = angles.size()-2;
  GLfloat offsetY = delHTX > delHTY? (1.0f / quadNum) * (delHTX/PAINT_WIDTH): (1.0f / quadNum) * (delHTY/PAINT_HEIGHT); //300是paint这个widget的宽度
  GLfloat uvOffsetY = 1.0f / quadNum;
  for(GLuint i = 0; i < quadNum; ++i){
  //pos
    QMatrix4x4 model1, model2, model3;
    model1.rotate(angles[i], QVector3D(1.0f, 0.0f, 0.0f));
    model2.rotate(angles[i+1], QVector3D(1.0f, 0.0f, 0.0f));
    model3.rotate(angles[i+2], QVector3D(1.0f, 0.0f, 0.0f));


    positions.push_back(matMulvec(model1, QVector3D(-0.5f, offsetY * i, 0.0f)));      //0
    positions.push_back(matMulvec(model1, QVector3D(0.5f, offsetY * i, 0.0f)));      //1
    positions.push_back(matMulvec(model2, QVector3D(-0.5f, offsetY * (i+1), 0.0f)));  //2

    positions.push_back(matMulvec(model2, QVector3D(0.5f, offsetY * (i+1), 0.0f)));  //3
    positions.push_back(matMulvec(model2, QVector3D(-0.5f, offsetY * (i+1), 0.0f)));  //4
    positions.push_back(matMulvec(model1, QVector3D(0.5f, offsetY * i, 0.0f)));      //5

    //uv
    uvs.push_back(QVector2D(0.0f, uvOffsetY * i));
    uvs.push_back(QVector2D(1.0f, uvOffsetY * i));
    uvs.push_back(QVector2D(0.0f, uvOffsetY * (i+1)));

    uvs.push_back(QVector2D(1.0f, uvOffsetY * (i+1)));
    uvs.push_back(QVector2D(0.0f, uvOffsetY * (i+1)));
    uvs.push_back(QVector2D(1.0f, uvOffsetY * i));

    //normals;
    if(isLeft){
      normals.push_back(matMulvec(model2, QVector3D(0.0, 0.0, 1.0)));
      normals.push_back(matMulvec(model2, QVector3D(0.0, 0.0, 1.0)));
      normals.push_back(matMulvec(model3, QVector3D(0.0, 0.0, 1.0)));

      normals.push_back(matMulvec(model3, QVector3D(0.0, 0.0, 1.0)));
      normals.push_back(matMulvec(model3, QVector3D(0.0, 0.0, 1.0)));
      normals.push_back(matMulvec(model2, QVector3D(0.0, 0.0, 1.0)));
    }else{
      normals.push_back(matMulvec(model2, QVector3D(0.0, 0.0, -1.0)));
      normals.push_back(matMulvec(model2, QVector3D(0.0, 0.0, -1.0)));
      normals.push_back(matMulvec(model3, QVector3D(0.0, 0.0, -1.0)));

      normals.push_back(matMulvec(model3, QVector3D(0.0, 0.0, -1.0)));
      normals.push_back(matMulvec(model3, QVector3D(0.0, 0.0, -1.0)));
      normals.push_back(matMulvec(model2, QVector3D(0.0, 0.0, -1.0)));
    }
  }

  //计算每个点的正副切线，此时点的法线，正副切线 没有柔和
  QVector<QVector3D> tangents, bitangents;
  computeTangentBasis(positions, uvs, normals, tangents, bitangents);

  //柔和重复点的法线与正副切线，同时获得索引点
  indexVBO_TBN(positions, uvs, normals, tangents, bitangents,
               indices, indexed_positions, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents);

  //向buffer赋值

  //positions
  core->glGenBuffers(1, &posVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, posVBO);
  core->glBufferData(GL_ARRAY_BUFFER, indexed_positions.size() * sizeof(QVector3D), &indexed_positions[0], GL_STATIC_DRAW);

  //uvs
  core->glGenBuffers(1, &uvVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
  core->glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(QVector3D), &indexed_uvs[0], GL_STATIC_DRAW);

  //normals
  core->glGenBuffers(1, &norVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, norVBO);
  core->glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(QVector3D), &indexed_normals[0], GL_STATIC_DRAW);

  //tangents
  core->glGenBuffers(1, &tanVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, tanVBO);
  core->glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(QVector3D), &indexed_tangents[0], GL_STATIC_DRAW);

  //bitangents
  core->glGenBuffers(1, &bitVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, bitVBO);
  core->glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(QVector3D), &indexed_bitangents[0], GL_STATIC_DRAW);

  //index
  core->glGenBuffers(1, &elementVBO);

  core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementVBO);
  core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);


//  for(int i = 0; i < indexed_positions.size(); ++i){
//      qDebug() << indexed_positions[i] << endl;
//  }
  return isLeft;
}

void Cube::initPlane(){
  /************/
  indices.clear();
  indexed_bitangents.clear();
  indexed_normals.clear();
  indexed_positions.clear();
  indexed_tangents.clear();
  indexed_uvs.clear();
  /************/

  QVector<QVector3D> positions;
  QVector<QVector2D> uvs;
  QVector<QVector3D> normals;



  positions.push_back(QVector3D(1.0f, 0.0f, 1.0f));      //0
  positions.push_back(QVector3D(-1.0f, 0.0f, -1.0f));      //0
  positions.push_back(QVector3D(-1.0f, 0.0f, 1.0f));      //0

  positions.push_back(QVector3D(1.0f, 0.0f, 1.0f));      //0
  positions.push_back(QVector3D(1.0f, 0.0f, -1.0f));      //0
  positions.push_back(QVector3D(-1.0f, 0.0f, -1.0f));      //0


  //uv
  uvs.push_back(QVector2D(1.0f, 0.0f));
  uvs.push_back(QVector2D(0.0f, 1.0f));
  uvs.push_back(QVector2D(0.0f, 0.0f));

  uvs.push_back(QVector2D(1.0f, 0.0f));
  uvs.push_back(QVector2D(1.0f, 1.0f));
  uvs.push_back(QVector2D(0.0f, 1.0f));

  normals.push_back(QVector3D(0.0, 1.0, 0.0));
  normals.push_back(QVector3D(0.0, 1.0, 0.0));
  normals.push_back(QVector3D(0.0, 1.0, 0.0));

  normals.push_back(QVector3D(0.0, 1.0, 0.0));
  normals.push_back(QVector3D(0.0, 1.0, 0.0));
  normals.push_back(QVector3D(0.0, 1.0, 0.0));

  //计算每个点的正副切线，此时点的法线，正副切线 没有柔和
  QVector<QVector3D> tangents, bitangents;
  computeTangentBasis(positions, uvs, normals, tangents, bitangents);

  //柔和重复点的法线与正副切线，同时获得索引点
  indexVBO_TBN(positions, uvs, normals, tangents, bitangents,
             indices, indexed_positions, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents);

//向buffer赋值

//positions
  core->glGenBuffers(1, &posVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, posVBO);
  core->glBufferData(GL_ARRAY_BUFFER, indexed_positions.size() * sizeof(QVector3D), &indexed_positions[0], GL_STATIC_DRAW);

//uvs
  core->glGenBuffers(1, &uvVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
  core->glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(QVector3D), &indexed_uvs[0], GL_STATIC_DRAW);

//normals
  core->glGenBuffers(1, &norVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, norVBO);
  core->glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(QVector3D), &indexed_normals[0], GL_STATIC_DRAW);

//tangents
  core->glGenBuffers(1, &tanVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, tanVBO);
  core->glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(QVector3D), &indexed_tangents[0], GL_STATIC_DRAW);

//bitangents
  core->glGenBuffers(1, &bitVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, bitVBO);
  core->glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(QVector3D), &indexed_bitangents[0], GL_STATIC_DRAW);

//index
  core->glGenBuffers(1, &elementVBO);

  core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementVBO);
  core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

}


void computeTangentBasis(QVector<QVector3D> &positions, QVector<QVector2D> &uvs, QVector<QVector3D> &normals, QVector<QVector3D> &tangents, QVector<QVector3D> &bitangents){
  for (int i=0; i != positions.size(); i+=3){
        // Shortcuts for vertices
    QVector3D &v0 = positions[i+0];
    QVector3D &v1 = positions[i+1];
    QVector3D &v2 = positions[i+2];

    // Shortcuts for UVs
    QVector2D &uv0 = uvs[i+0];
    QVector2D &uv1 = uvs[i+1];
    QVector2D &uv2 = uvs[i+2];

    // Edges of the triangle : postion delta
    QVector3D deltaPos1 = v1-v0;
    QVector3D deltaPos2 = v2-v0;

    // UV delta
    QVector2D deltaUV1 = uv1-uv0;
    QVector2D deltaUV2 = uv2-uv0;

    float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
    QVector3D tangent = (deltaPos1 * deltaUV2.y()   - deltaPos2 * deltaUV1.y()) * r;
    QVector3D bitangent = (deltaPos2 * deltaUV1.x()   - deltaPos1 * deltaUV2.x()) * r;
    tangent.normalized();
    bitangent.normalized();

    // Set the same tangent for all three vertices of the triangle.
    // They will be merged later, in vboindexer.cpp
    tangents.push_back(tangent);
    tangents.push_back(tangent);
    tangents.push_back(tangent);

    // Same thing for binormals
    bitangents.push_back(bitangent);
    bitangents.push_back(bitangent);
    bitangents.push_back(bitangent);
  }

  for (int i=0; i < positions.size(); i++)
  {
      QVector3D &n = normals[i];
      QVector3D &t = tangents[i];
      QVector3D &b = bitangents[i];

      // Gram-Schmidt orthogonalize
//      t = (t - QVector3D::dotProduct(t, n) * n).normalized();
      // Calculate handedness
//      if(QVector3D::dotProduct(QVector3D::crossProduct(n, t), b) < 0.0f)
//          t *= -1.0f;
//      b = QVector3D::crossProduct(t, n);

  }
}


GLboolean is_Near(GLfloat v1, GLfloat v2){
  return fabs(v1 - v2) < 0.01f;
}

// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same UVs + same normal
GLboolean getSimilarVertexIndex(QVector3D &in_positions, QVector<QVector3D> &out_positions, GLushort &result){
// Lame linear search
  for (int i=0; i < out_positions.size(); i++){
    if (is_Near( in_positions.x() , out_positions[i].x() ) && is_Near( in_positions.y() , out_positions[i].y() ) && is_Near( in_positions.z() , out_positions[i].z() ) ){
      result = i;
      return true;
    }
  }
    // No other vertex could be used instead.
    // Looks like we'll have to add it to the VBO.
  return false;
}

void indexVBO_TBN(
    QVector<QVector3D> &in_positions,
    QVector<QVector2D> &in_uvs,
    QVector<QVector3D> &in_normals,
    QVector<QVector3D> &in_tangents,
    QVector<QVector3D> &in_bitangents,

    QVector<GLushort> &out_indices,
    QVector<QVector3D> &out_positions,
    QVector<QVector2D> &out_uvs,
    QVector<QVector3D> &out_normals,
    QVector<QVector3D> &out_tangents,
    QVector<QVector3D> &out_bitangents
){
    // For each input vertex
  for (int i=0; i<in_positions.size(); i++ ){
        // Try to find a similar vertex in out_XXXX
    GLushort index;
    GLboolean found = getSimilarVertexIndex(in_positions[i], out_positions, index);

    if ( found ){ // A similar vertex is already in the VBO, use it instead !
      out_indices.push_back(index);

      // Average the tangents and the bitangents
      out_normals[index] += in_normals[i];
      out_tangents[index] += in_tangents[i];
      out_bitangents[index] += in_bitangents[i];
    }else{ // If not, it needs to be added in the output data.
      out_positions.push_back(in_positions[i]);
      out_uvs.push_back(in_uvs[i]);
      out_normals.push_back(in_normals[i]);
      out_tangents.push_back(in_tangents[i]);
      out_bitangents.push_back(in_bitangents[i]);
      out_indices.push_back((GLushort)out_positions.size()-1);
    }
  }
}

QVector3D matMulvec(QMatrix4x4 matrix, QVector3D vector) {
  QVector4D result = (matrix * QVector4D(vector, 1.0f));

  return QVector3D(result.x(), result.y(), result.z());
}

void Cube::drawCube(){
//  core->glActiveTexture(GL_TEXTURE0);
//  ResourceManager::getTexture("brickwall").bind();

//  core->glActiveTexture(GL_TEXTURE1);
//  ResourceManager::getTexture("brickwall_normal").bind();

//  core->glBindVertexArray(cubeVAO);//我操你妈！！！！ VAO坏了 日日日日日 fuckfuckfuck
  core->glEnableVertexAttribArray(0);
  core->glBindBuffer(GL_ARRAY_BUFFER, posVBO);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

  core->glEnableVertexAttribArray(1);
  core->glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
  core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

  core->glEnableVertexAttribArray(2);
  core->glBindBuffer(GL_ARRAY_BUFFER, norVBO);
  core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

  core->glEnableVertexAttribArray(3);
  core->glBindBuffer(GL_ARRAY_BUFFER, tanVBO);
  core->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

  core->glEnableVertexAttribArray(4);
  core->glBindBuffer(GL_ARRAY_BUFFER, bitVBO);
  core->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

  core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementVBO);
  core->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
}



/**************            LIGHT             ************************/

Light::Light(){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Light::~Light(){
  core->glDeleteBuffers(1, &lightVBO);
}

void Light::init(){

  float lightVertices[] = {
      // positions          // normals           // texture coords
      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,

      -0.5f, -0.5f,  0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,

      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,

       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,

      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f, -0.5f,

      -0.5f,  0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f
  };

  core->glGenBuffers(1, &lightVBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);


}

void Light::drawLight(){
  core->glEnableVertexAttribArray(0);
  core->glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  core->glDrawArrays(GL_TRIANGLES, 0, 36);
}

/**************            COORDINATE             ************************/

Coordinate::Coordinate(){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Coordinate::~Coordinate(){
  core->glDeleteBuffers(1, &VBO);
}

void Coordinate::init(){

  float vertices[] = {
      // positions          // normals           // texture coords
      0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,

      0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,

      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f
  };

  core->glGenBuffers(1, &VBO);

  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


}

void Coordinate::draw(){
  core->glEnableVertexAttribArray(0);
  core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
  core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  core->glDrawArrays(GL_LINES, 0, 6);
}


///**************            FLOOR             ************************/

Floor::Floor(){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

Floor::~Floor(){
  core->glDeleteBuffers(1, &VBO);
}

void Floor::init(){
    float vertices[] = {
        // positions          // normals           // texture coords
      1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

      1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    core->glGenBuffers(1, &VBO);

    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Floor::draw(){
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    core->glEnableVertexAttribArray(0);
//    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    core->glEnableVertexAttribArray(2);
    core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    core->glDrawArrays(GL_TRIANGLES, 0, 6);
}
