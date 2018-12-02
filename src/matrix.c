#include "matrix.h"

void print_vec4(vec4 vect) { // print a 4x1 vec4
	printf("[%.2f, %.2f, %.2f, %.2f]\n", vect.x, vect.y, vect.z, vect.w);
}

void print_mat4(mat4 mat) {
	//vec4 a, b, c, d;

	/*
	a.x = mat.x.x; a.y = mat.y.x; a.z = mat.z.x; a.w = mat.w.x;
	b.x = mat.x.y; b.y = mat.y.y; b.z = mat.z.y; b.w = mat.w.y;
	c.x = mat.x.z; c.y = mat.y.z; c.z = mat.z.z; c.w = mat.w.z;
	d.x = mat.x.w; d.y = mat.y.w; d.z = mat.z.w; d.w = mat.w.w;
	*/

	mat4 temp = transpose(mat);

	printf("\n");
	print_vec4(temp.x);
	print_vec4(temp.y);
	print_vec4(temp.z);
	print_vec4(temp.w);
}

vec4 v_add(vec4 a, vec4 b) { // vec4-vec4 addition
	vec4 rvec;
	rvec.x = a.x + b.x;
	rvec.y = a.y + b.y;
	rvec.z = a.z + b.z;
	rvec.w = a.w + b.w;
	return rvec;
}

vec4 v_sub(vec4 a, vec4 b) { // vec4-vec4 subtraction
	vec4 rvec;
	rvec.x = a.x - b.x;
	rvec.y = a.y - b.y;
	rvec.z = a.z - b.z;
	rvec.w = a.w - b.w;
	return rvec;
}

vec4 svect_mult(GLfloat alpha, vec4 v) { // scalar-vec4 multiplication
	vec4 rv;
	rv.x = v.x*alpha;
	rv.y = v.y*alpha;
	rv.z = v.z*alpha;
	rv.w = v.w*alpha;
	return rv;
}

GLfloat v_dot(vec4 a, vec4 b) { // vec4 dot product
	return (GLfloat)(a.x*b.x) + (a.y*b.y) + (a.z*b.z) + (a.w*b.w); // superfluous cast?
}

vec4 v_cross(vec4 a, vec4 b) { // vec4 cross product
	vec4 rvec;
	rvec.x = (a.y * b.z) - (a.z * b.y);
	rvec.y = (a.z * b.x) - (a.x * b.z);
	rvec.z = (a.x * b.y) - (a.y * b.x);
	rvec.w = 0;
	return rvec;
}

mat4 identity_matrix() {
	mat4 rmat;
	rmat.x.x = 1;   rmat.y.x = 0;   rmat.z.x = 0;   rmat.w.x = 0;
	rmat.x.y = 0;   rmat.y.y = 1;   rmat.z.y = 0;   rmat.w.y = 0;
	rmat.x.z = 0;   rmat.y.z = 0;   rmat.z.z = 1;   rmat.w.z = 0;
	rmat.x.w = 0;   rmat.y.w = 0;   rmat.z.w = 0;   rmat.w.w = 1;
	return rmat;
}

mat4 smat_mult(GLfloat alpha, mat4 m) {
	mat4 rmat;
	rmat.x = svect_mult(alpha, m.x);
	rmat.y = svect_mult(alpha, m.y);
	rmat.z = svect_mult(alpha, m.z);
	rmat.w = svect_mult(alpha, m.w);
	return rmat;
}

mat4 m_add(mat4 m, mat4 n) {
	mat4 rmat;
	rmat.x = v_add(m.x, n.x);
	rmat.y = v_add(m.y, n.y);
	rmat.z = v_add(m.z, n.z);
	rmat.w = v_add(m.w, n.w);
	return rmat;
}

mat4 m_sub(mat4 m, mat4 n) {
	mat4 rmat;
	rmat.x = v_sub(m.x, n.x);
	rmat.y = v_sub(m.y, n.y);
	rmat.z = v_sub(m.z, n.z);
	rmat.w = v_sub(m.w, n.w);
	return rmat;
}

mat4 m_mult(mat4 a, mat4 b) {
	mat4 rmat;

	rmat.x.x = (a.x.x * b.x.x) + (a.x.y * b.y.x) + (a.x.z * b.z.x) + (a.x.w * b.w.x);
	rmat.y.x = (a.y.x * b.x.x) + (a.y.y * b.y.x) + (a.y.z * b.z.x) + (a.y.w * b.w.x);
	rmat.z.x = (a.z.x * b.x.x) + (a.z.y * b.y.x) + (a.z.z * b.z.x) + (a.z.w * b.w.x);
	rmat.w.x = (a.w.x * b.x.x) + (a.w.y * b.y.x) + (a.w.z * b.z.x) + (a.w.w * b.w.x);

	rmat.x.y = (a.x.x * b.x.y) + (a.x.y * b.y.y) + (a.x.z * b.z.y) + (a.x.w * b.w.y);
	rmat.y.y = (a.y.x * b.x.y) + (a.y.y * b.y.y) + (a.y.z * b.z.y) + (a.y.w * b.w.y);
	rmat.z.y = (a.z.x * b.x.y) + (a.z.y * b.y.y) + (a.z.z * b.z.y) + (a.z.w * b.w.y);
	rmat.w.y = (a.w.x * b.x.y) + (a.w.y * b.y.y) + (a.w.z * b.z.y) + (a.w.w * b.w.y);

	rmat.x.z = (a.x.x * b.x.z) + (a.x.y * b.y.z) + (a.x.z * b.z.z) + (a.x.w * b.w.z);
	rmat.y.z = (a.y.x * b.x.z) + (a.y.y * b.y.z) + (a.y.z * b.z.z) + (a.y.w * b.w.z);
	rmat.z.z = (a.z.x * b.x.z) + (a.z.y * b.y.z) + (a.z.z * b.z.z) + (a.z.w * b.w.z);
	rmat.w.z = (a.w.x * b.x.z) + (a.w.y * b.y.z) + (a.w.z * b.z.z) + (a.w.w * b.w.z);

	rmat.x.w = (a.x.x * b.x.w) + (a.x.y * b.y.w) + (a.x.z * b.z.w) + (a.x.w * b.w.w);
	rmat.y.w = (a.y.x * b.x.w) + (a.y.y * b.y.w) + (a.y.z * b.z.w) + (a.y.w * b.w.w);
	rmat.z.w = (a.z.x * b.x.w) + (a.z.y * b.y.w) + (a.z.z * b.z.w) + (a.z.w * b.w.w);
	rmat.w.w = (a.w.x * b.x.w) + (a.w.y * b.y.w) + (a.w.z * b.z.w) + (a.w.w * b.w.w);

	return rmat;
}

mat4 transpose(mat4 a) {
	mat4 rmat;

	rmat.x.x = a.x.x;
	rmat.x.y = a.y.x;
	rmat.x.z = a.z.x;
	rmat.x.w = a.w.x;

	rmat.y.x = a.x.y;
	rmat.y.y = a.y.y;
	rmat.y.z = a.z.y;
	rmat.y.w = a.w.y;

	rmat.z.x = a.x.z;
	rmat.z.y = a.y.z;
	rmat.z.z = a.z.z;
	rmat.z.w = a.w.z;

	rmat.w.x = a.x.w;
	rmat.w.y = a.y.w;
	rmat.w.z = a.z.w;
	rmat.w.w = a.w.w;

	return rmat;
}

int inverse(mat4 a, mat4* rmat) {

	GLfloat out[16], m[16], det;
	//int i;

	m[0] = a.x.x;
	m[1] = a.x.y;
	m[2] = a.x.z;
	m[3] = a.x.w;

	m[4] = a.y.x;
	m[5] = a.y.y;
	m[6] = a.y.z;
	m[7] = a.y.w;

	m[8] = a.z.x;
	m[9] = a.z.y;
	m[10] = a.z.z;
	m[11] = a.z.w;

	m[12] = a.w.x;
	m[13] = a.w.y;
	m[14] = a.w.z;
	m[15] = a.w.w;

	out[0] = m[5] * m[10] * m[15] -
			 m[5] * m[11] * m[14] -
			 m[9] * m[6] * m[15] +
			 m[9] * m[7] * m[14] +
			 m[13] * m[6] * m[11] -
			 m[13] * m[7] * m[10];

	out[4] = -m[4] * m[10] * m[15] +
			  m[4] * m[11] * m[14] +
			  m[8] * m[6] * m[15] -
			  m[8] * m[7] * m[14] -
			  m[12] * m[6] * m[11] +
			  m[12] * m[7] * m[10];

	out[8] = m[4] * m[9] * m[15] -
			 m[4] * m[11] * m[13] -
			 m[8] * m[5] * m[15] +
			 m[8] * m[7] * m[13] +
			 m[12] * m[5] * m[11] -
			 m[12] * m[7] * m[9];

	out[12] = -m[4] * m[9] * m[14] +
			   m[4] * m[10] * m[13] +
			   m[8] * m[5] * m[14] -
			   m[8] * m[6] * m[13] -
			   m[12] * m[5] * m[10] +
			   m[12] * m[6] * m[9];

	out[1] = -m[1] * m[10] * m[15] +
			  m[1] * m[11] * m[14] +
			  m[9] * m[2] * m[15] -
			  m[9] * m[3] * m[14] -
			  m[13] * m[2] * m[11] +
			  m[13] * m[3] * m[10];

	out[5] = m[0] * m[10] * m[15] -
			 m[0] * m[11] * m[14] -
			 m[8] * m[2] * m[15] +
			 m[8] * m[3] * m[14] +
			 m[12] * m[2] * m[11] -
			 m[12] * m[3] * m[10];

	out[9] = -m[0] * m[9] * m[15] +
			  m[0] * m[11] * m[13] +
			  m[8] * m[1] * m[15] -
			  m[8] * m[3] * m[13] -
			  m[12] * m[1] * m[11] +
			  m[12] * m[3] * m[9];

	out[13] = m[0] * m[9] * m[14] -
			  m[0] * m[10] * m[13] -
			  m[8] * m[1] * m[14] +
			  m[8] * m[2] * m[13] +
			  m[12] * m[1] * m[10] -
			  m[12] * m[2] * m[9];

	out[2] = m[1] * m[6] * m[15] -
			 m[1] * m[7] * m[14] -
			 m[5] * m[2] * m[15] +
			 m[5] * m[3] * m[14] +
			 m[13] * m[2] * m[7] -
			 m[13] * m[3] * m[6];

	out[6] = -m[0] * m[6] * m[15] +
			  m[0] * m[7] * m[14] +
			  m[4] * m[2] * m[15] -
			  m[4] * m[3] * m[14] -
			  m[12] * m[2] * m[7] +
			  m[12] * m[3] * m[6];

	out[10] = m[0] * m[5] * m[15] -
			  m[0] * m[7] * m[13] -
			  m[4] * m[1] * m[15] +
			  m[4] * m[3] * m[13] +
			  m[12] * m[1] * m[7] -
			  m[12] * m[3] * m[5];

	out[14] = -m[0] * m[5] * m[14] +
			   m[0] * m[6] * m[13] +
			   m[4] * m[1] * m[14] -
			   m[4] * m[2] * m[13] -
			   m[12] * m[1] * m[6] +
			   m[12] * m[2] * m[5];

	out[3] = -m[1] * m[6] * m[11] +
			  m[1] * m[7] * m[10] +
			  m[5] * m[2] * m[11] -
			  m[5] * m[3] * m[10] -
			  m[9] * m[2] * m[7] +
			  m[9] * m[3] * m[6];

	out[7] =  m[0] * m[6] * m[11] -
			  m[0] * m[7] * m[10] -
			  m[4] * m[2] * m[11] +
			  m[4] * m[3] * m[10] +
			  m[8] * m[2] * m[7] -
			  m[8] * m[3] * m[6];

	out[11] = -m[0] * m[5] * m[11] +
			   m[0] * m[7] * m[9] +
			   m[4] * m[1] * m[11] -
			   m[4] * m[3] * m[9] -
			   m[8] * m[1] * m[7] +
			   m[8] * m[3] * m[5];

	out[15] =  m[0] * m[5] * m[10] -
			   m[0] * m[6] * m[9] -
			   m[4] * m[1] * m[10] +
			   m[4] * m[2] * m[9] +
			   m[8] * m[1] * m[6] -
			   m[8] * m[2] * m[5];

	det = m[0] * out[0] + m[1] * out[4] + m[2] * out[8] + m[3] * out[12];

	if (det == 0)
		return 0;

	det = 1.0 / det;

	/*for (i = 0; i < 16; i++)
	outout[i] = out[i] * det;*/

	rmat->x.x = out[0] * det;
	rmat->x.y = out[1] * det;
	rmat->x.z = out[2] * det;
	rmat->x.w = out[3] * det;
	rmat->y.x = out[4] * det;
	rmat->y.y = out[5] * det;
	rmat->y.z = out[6] * det;
	rmat->y.w = out[7] * det;
	rmat->z.x = out[8] * det;
	rmat->z.y = out[9] * det;
	rmat->z.z = out[10] * det;
	rmat->z.w = out[11] * det;
	rmat->w.x = out[12] * det;
	rmat->w.y = out[13] * det;
	rmat->w.z = out[14] * det;
	rmat->w.w = out[15] * det;

	return 1;
}
/*
vec4 mat_vec_mult(mat4 m, vec4 v) {
	vec4 rvec;

	//m = transpose(m);

	rvec.x = v_dot(m.x, v);
	rvec.y = v_dot(m.y, v);
	rvec.z = v_dot(m.z, v);
	rvec.w = v_dot(m.w, v);

	return rvec;
}*/

vec4 mat_vec_mult(mat4 m, vec4 v) {
	vec4 rvec;

	rvec = svect_mult(v.x, m.x);
	rvec = v_add(svect_mult(v.y, m.y), rvec);
	rvec = v_add(svect_mult(v.z, m.z), rvec);
	rvec = v_add(svect_mult(v.w, m.w), rvec);

	return rvec;	
}

mat4 translation_matrix(GLfloat x, GLfloat y, GLfloat z) {
	mat4 rmat = identity_matrix();

	rmat.w.x = x;
	rmat.w.y = y;
	rmat.w.z = z;

	return rmat;
}

mat4 scale_matrix(GLfloat Bx, GLfloat By, GLfloat Bz) {
	mat4 rmat = identity_matrix();

	rmat.x.x = Bx;
	rmat.y.y = By;
	rmat.z.z = Bz;

	return rmat;
}

mat4 x_rot_matrix(GLfloat theta)
{
	mat4 rmat = identity_matrix();

	rmat.y.y = cos(theta);
	rmat.y.z = sin(theta);
	rmat.z.y = -sin(theta);
	rmat.z.z = cos(theta);

	return rmat;
}

mat4 y_rot_matrix(GLfloat theta)
{
	mat4 rmat = identity_matrix();

	rmat.x.x = cos(theta);
	rmat.x.z = -sin(theta);
	rmat.z.x = sin(theta);
	rmat.z.z = cos(theta);

	return rmat;
}

mat4 z_rot_matrix(GLfloat theta)
{
	mat4 rmat = identity_matrix();

	rmat.x.x = cos(theta);
	rmat.x.y = sin(theta);
	rmat.y.x = -sin(theta);
	rmat.y.y = cos(theta);

	return rmat;
}

GLfloat v4_magnitude(vec4 v) {
    GLfloat result = 0.0;
    result += v.x * v.x;
    result += v.y * v.y;
    result += v.z * v.z;
    result = sqrt(result);
    return result;
}

vec4 unit_vector(vec4 v) {
    GLfloat magnitude = v4_magnitude(v);
    return svect_mult(1.0f/magnitude, v);
}

mat4 look_at(GLfloat eyex, GLfloat eyey, GLfloat eyez,
			 GLfloat atx, GLfloat aty, GLfloat atz,
			 GLfloat upx, GLfloat upy, GLfloat upz)
{
	vec4 vup = {upx, upy, upz, 0};
    
    vec4 e = {eyex, eyey, eyez, 0};
    vec4 a = {atx, aty, atz, 0};
    
    
    vec4 n = v_sub(a, e);   
    n = unit_vector(n);
    
    vec4 u = v_cross(n, vup);
    u = unit_vector(u);
       
    vec4 v = v_cross(u, n);
    v = unit_vector(v);
    
    // Change sign
    n = svect_mult(-1.0f, n);
    
    mat4 initial_matrix = {
        u, v, n, {0, 0, 0, 0}
    };
    
    // print_mat4(initial_matrix);
    
    mat4 transposed_matrix = transpose(initial_matrix);
    
    transposed_matrix.w.x = -1.0f * v_dot(u, e);
    transposed_matrix.w.y = -1.0f * v_dot(v, e);
    transposed_matrix.w.z = -1.0f * v_dot(n, e);
    transposed_matrix.w.w = 1;
    
    // print_mat4(transposed_matrix);
    
    return transposed_matrix;
}

mat4 frustum(GLfloat left, GLfloat right, GLfloat bottom, 
			 GLfloat top, GLfloat near, GLfloat far)
{
	mat4 frustum = identity_matrix();
    
    frustum.x.x = (-2.0f * near) / (right - left);
    frustum.y.y = (-2.0f * near) / (top - bottom);
    frustum.z.x = (left + right) / (right - left);
    frustum.z.y = (bottom + top) / (top - bottom);
    frustum.z.z = (near + far) / (far - near);
    frustum.z.w = -1.0f;
    frustum.w.z = -1.0f*(2.0f * near * far) / (far - near);
    frustum.w.w = 0.0;

    // print_mat4(frustum);

    return frustum;
}
