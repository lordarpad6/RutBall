#include "Ball.h"


Ball::~Ball(void)
{
}

Ball::Ball(float radius, Point center)
	: Drawable(center)
{
			float const R = 1./(float)(rings-1);
			float const S = 1./(float)(sectors-1);
			int r, s;

			vertices.resize(rings * sectors * 3);
			normals.resize(rings * sectors * 3);
			texcoords.resize(rings * sectors * 2);
			std::vector<GLfloat>::iterator v = vertices.begin();
			std::vector<GLfloat>::iterator n = normals.begin();
			std::vector<GLfloat>::iterator t = texcoords.begin();
			for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
					float const y = sin( -PI/2 +PI * r * R );
					float const x = cos(2*PI * s * S) * sin( PI * r * R );
					float const z = sin(2*PI * s * S) * sin( PI * r * R );

					*t++ = s*S;
					*t++ = r*R;

					*v++ = x * radius;
					*v++ = y * radius;
					*v++ = z * radius;

					*n++ = x;
					*n++ = y;
					*n++ = z;
			}

			indices.resize(rings * sectors * 4);
			std::vector<GLushort>::iterator i = indices.begin();
			for(r = 0; r < rings-1; r++) 
				for(s = 0; s < sectors-1; s++) {
					*i++ = r * sectors + s;
					*i++ = r * sectors + (s+1);
					*i++ = (r+1) * sectors + (s+1);
					*i++ = (r+1) * sectors + s;
			}
}

void Ball::Draw()
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
		glTranslatef(this->center.x, this->center.y, this->center.z);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glNormalPointer(GL_FLOAT, 0, &normals[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
        glPopMatrix();
}