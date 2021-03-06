#include "sphere.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

#define PI 3.14159265


extern float grid;
extern Point boardcenter;
extern float theta;
float s = sin(theta * PI / 180);
float c = cos(theta * PI / 180);
/**********************************************************************
 * This function intersects a ray with a given sphere 'sph'. You should
 * use the parametric representation of a line and do the intersection.
 * The function should return the parameter value for the intersection, 
 * which will be compared with others to determine which intersection
 * is closest. The value -1.0 is returned if there is no intersection
 *
 * If there is an intersection, the point of intersection should be
 * stored in the "hit" variable
 **********************************************************************/
float intersect_sphere(Point o, Vector u, Spheres *sph, Point *hit, int type) {
  float x1 = o.x - sph->center.x;
  float y1 = o.y - sph->center.y;
  float z1 = o.z - sph->center.z;
  float a = u.x * u.x + u.y * u.y + u.z * u.z;
  float b = 2.0 * (u.x * x1 + u.y * y1 + u.z * z1);
  float c = x1 * x1 + y1 * y1 + z1 * z1 - (sph->radius) * (sph->radius);
  float delta = b * b - 4 * a * c;
  if (delta < 0)
    return -1.0;
  float t1 = (-b - sqrt(delta)) / (a * 2);
  float t2 = (-b + sqrt(delta)) / (a * 2);
  float t;
  if (type == 1)
    t = t1;
  else
    t = t2;
  //std::cout <<t<<std::endl;
  hit->x = o.x + t * u.x;
  hit->y = o.y + t * u.y;
  hit->z = o.z + t * u.z;
	return t;
}

bool intersect_board(Point o, Vector u, float *x, float *y, Point *hit)
{
  float t = (s * boardcenter.z + c * boardcenter.y - s * o.z - c * o.y) / (s * u.z + c * u.y);
  if (t < 0)
    return false;
  hit->x = o.x + t * u.x;
  hit->y = o.y + t * u.y;
  hit->z = o.z + t * u.z;
  *x = (o.x + t * u.x - boardcenter.x) / grid;
  *y = (o.y + t * u.y - boardcenter.y) / (s * grid);
  return ((abs(*x) < 4.0) && (abs(*y) < 4.0));
}
/*********************************************************************
 * This function returns a pointer to the sphere object that the
 * ray intersects first; NULL if no intersection. You should decide
 * which arguments to use for the function. For exmaple, note that you
 * should return the point of intersection to the calling function.
 **********************************************************************/
Spheres *intersect_scene(Point o, Vector u, Spheres *sph, Point *hit, int type) {
  Spheres *head = sph;
  Spheres *re = NULL;
  Point *temp = new Point;
  float closest = 500;

  while (head) {
    float t = intersect_sphere(o, u, head, temp, type);
    if (t > 0.00001 && t < closest) {
      closest = t;
      *hit = *temp;
      re = head;
    } 
    head = head->next;
  }
  delete temp;
  return re;
}

/*****************************************************
 * This function adds a sphere into the sphere list
 *
 * You need not change this.
 *****************************************************/
Spheres *add_sphere(Spheres *slist, Point ctr, float rad, float amb[],
        float dif[], float spe[], float shine, 
        float refl, float refr, float eta, int sindex) {
  Spheres *new_sphere;

  new_sphere = (Spheres *)malloc(sizeof(Spheres));
  new_sphere->index = sindex;
  new_sphere->center = ctr;
  new_sphere->radius = rad;
  (new_sphere->mat_ambient)[0] = amb[0];
  (new_sphere->mat_ambient)[1] = amb[1];
  (new_sphere->mat_ambient)[2] = amb[2];
  (new_sphere->mat_diffuse)[0] = dif[0];
  (new_sphere->mat_diffuse)[1] = dif[1];
  (new_sphere->mat_diffuse)[2] = dif[2];
  (new_sphere->mat_specular)[0] = spe[0];
  (new_sphere->mat_specular)[1] = spe[1];
  (new_sphere->mat_specular)[2] = spe[2];
  new_sphere->mat_shineness = shine;
  new_sphere->reflectance = refl;
  new_sphere->refraction = refr;
  new_sphere->eta = eta;
  new_sphere->next = NULL;

  if (slist == NULL) { // first object
    slist = new_sphere;
  } else { // insert at the beginning
    new_sphere->next = slist;
    slist = new_sphere;
  }

  return slist;
}
/******************************************
 * computes a sphere normal - done for you
 ******************************************/
Vector sphere_normal(Point q, Spheres *sph) {
  Vector rc;

  rc = get_vec(sph->center, q);
  normalize(&rc);
  return rc;
}
