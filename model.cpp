#include "model.h"
bool loadOBJ(const char *path, QVector<QVector3D> &out_positions, QVector<QVector2D> &out_uvs, QVector<QVector3D> &out_normals);


Model::Model(){
  core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

}

bool Model::init(const char *path){
  bool res = loadOBJ(path, positions, uvs, normals);

  core->glGenBuffers(1, &positionVBO);
  core->glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
  core->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(QVector3D), &positions[0], GL_STATIC_DRAW);

  core->glGenBuffers(1, &uvVBO);
  core->glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
  core->glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(QVector2D), &uvs[0], GL_STATIC_DRAW);

  core->glGenBuffers(1, &normalVBO);
  core->glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
  core->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(QVector3D), &normals[0], GL_STATIC_DRAW);

//  qDebug() << " normal size: " << normals.size() << endl;
//  for(int i = 0; i < normals.size(); ++i)
//      qDebug() << normals[i] << endl;

  return res;
}

void Model::draw(){
  core->glEnableVertexAttribArray(0);
  core->glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
  core->glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // 2nd attribute buffer : UVs
  core->glEnableVertexAttribArray(1);
  core->glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
  core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  core->glEnableVertexAttribArray(2);
  core->glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
  core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Draw the triangle !
  core->glDrawArrays(GL_TRIANGLES, 0, positions.size());
}

bool loadOBJ(const char *path, QVector<QVector3D> &out_positions, QVector<QVector2D> &out_uvs, QVector<QVector3D> &out_normals)
{
    //printf("Loading OBJ file %s...\n", path);

    QVector<unsigned int> vertexIndices, uvIndices, normalIndices;
    QVector<QVector3D> temp_vertices;
    QVector<QVector2D> temp_uvs;
    QVector<QVector3D> temp_normals;


    FILE * file = fopen(path, "r");
    if( file == NULL ){
      qDebug() << "Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n" << endl;
      return false;
    }

    while( 1 ){
      char lineHeader[128];
      // read the first word of the line
      int res = fscanf(file, "%s", lineHeader);
      if (res == EOF)
        break; // EOF = End Of File. Quit the loop.

            // else : parse lineHeader

      if (strcmp(lineHeader, "v") == 0){
        QVector3D pos;
        float x, y, z;
        fscanf(file, "%f %f %f\n", &x, &y, &z );
        pos.setX(x);
        pos.setY(y);
        pos.setZ(z);
        temp_vertices.push_back(pos);
      }else if (strcmp(lineHeader, "vt") == 0){
         QVector2D uv;
         float x, y;
         fscanf(file, "%f %f\n", &x, &y);
         y = -y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
         uv.setX(x);
         uv.setY(y);
         temp_uvs.push_back(uv);
      }else if (strcmp(lineHeader, "vn") == 0){
         QVector3D normal;
         float x, y, z;
         fscanf(file, "%f %f %f\n", &x, &y, &z );
         normal.setX(x);
         normal.setY(y);
         normal.setZ(z);
         temp_normals.push_back(normal);
       }else if (strcmp( lineHeader, "f") == 0){
         QString vertex1, vertex2, vertex3;
         unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
         int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
         if (matches != 9){
           qDebug() << "File can't be read by our simple parser :-( Try exporting with other options\n" << endl;
           fclose(file);
           return false;
         }
         vertexIndices.push_back(vertexIndex[0]);
         vertexIndices.push_back(vertexIndex[1]);
         vertexIndices.push_back(vertexIndex[2]);
         uvIndices    .push_back(uvIndex[0]);
         uvIndices    .push_back(uvIndex[1]);
         uvIndices    .push_back(uvIndex[2]);
         normalIndices.push_back(normalIndex[0]);
         normalIndices.push_back(normalIndex[1]);
         normalIndices.push_back(normalIndex[2]);
       }else{
                // Probably a comment, eat up the rest of the line
         char stupidBuffer[1000];
         fgets(stupidBuffer, 1000, file);
       }

    }

        // For each vertex of each triangle
    for(int i=0; i < vertexIndices.size(); i++ ){
            // Get the indices of its attributes
      unsigned int vertexIndex = vertexIndices[i];
      unsigned int uvIndex = uvIndices[i];
      unsigned int normalIndex = normalIndices[i];

            // Get the attributes thanks to the index
      QVector3D vertex = temp_vertices[ vertexIndex-1 ];
      QVector2D uv = temp_uvs[ uvIndex-1 ];
      QVector3D normal = temp_normals[ normalIndex-1 ];

            // Put the attributes in buffers
      out_positions.push_back(vertex);
      out_uvs     .push_back(uv);
      out_normals .push_back(normal);

    }
    fclose(file);
    return true;
}
