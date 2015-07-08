/* algoritms for calc azimuth uses structure */
const double PI = 3.14159265358979323846;

const double rad90 = 1.5707963267948966192313216916398;


typedef struct{
	double x;
	double y;
	double z;
	double D; // for subtraction and summ vector
} vector_coord;

typedef struct{
	double A;
	double B;
	double C;
	double D;
} plane;

double scal_vect(vector_coord first, vector_coord second){
	return first.x*second.x + first.y*second.y + first.z*second.z;	//возращаем скалярное произведение
}

double rootEquation(plane centre, vector_coord target){
	double ax = centre.A*target.x;
	double by = centre.B*target.y;
	double cz = centre.C*target.z;
	double abc_xyz_D = ax + by + cz - centre.D;
	double a2 = centre.A*centre.A;
	double b2 = centre.B*centre.B;
	double c2 = centre.C*centre.C;
	double a2_b2_c2 = a2 + b2 + c2;
	return -abc_xyz_D/a2_b2_c2;
}

vector_coord summVector(vector_coord first, vector_coord second){
	vector_coord out;
	out.x = first.x + second.x;
	out.y = first.y + second.y;
	out.z = first.z + second.z;
	out.D = sqrt(out.x*out.x + out.y*out.y + out.z*out.z);
	return out;
}

vector_coord subtractVector(vector_coord first, vector_coord second){
	vector_coord out;
	out.x = first.x - second.x;
	out.y = first.y - second.y;
	out.z = first.z - second.z;
	out.D = sqrt(out.x*out.x + out.y*out.y + out.z*out.z);
	return out;
}

/* от начала координат */
double lenghtVector(vector_coord vect){
	return sqrt(vect.x*vect.x + vect.y*vect.y + vect.z*vect.z);
}

vector_coord vectorProduct(vector_coord first, vector_coord second){
	vector_coord out;
	out.x = first.y*second.z - first.z*second.y;
	out.y = first.z*second.x - first.x*second.z;
	out.z = first.x*second.y - first.y*second.x;
	return out;
}

vector_coord vectorNorm(vector_coord input){
	double D = lenghtVector(input);

	vector_coord norm;
	norm.x = input.x/D;
	norm.y = input.y/D;
	norm.z = input.z/D;
	norm.D = lenghtVector(norm);	// должен быть единичный
	return norm;
}

//===================================================================
//			Вычисление угла между векторами (в радианах)
//===================================================================
double angleVectors(vector_coord first, vector_coord second){
	double scalarProduct = scal_vect(first, second);
	/* проверка на нулевые вектора */
	if((first.D == 0) || (second.D == 0))
		return 0;
	/* проверка на ортоганальность */
	return scalarProduct ? acos(scalarProduct/(first.D*second.D)) : rad90;
}

double azimuthCalc(vector_coord centre, vector_coord target){
	/* проверка на перпендикулярности вектора centre - оси Оz */
	if(centre.x == 0 && centre.y == 0)
		return 0;

	centre.x == target.x;
	centre.y == target.y;
	centre.z == target.z;

	/* Уравнение плоскости Ax+By+Cz-D = 0 */
	plane planeCentre;
	planeCentre.A = centre.x;
	planeCentre.B = centre.y;
	planeCentre.C = centre.z;
	planeCentre.D = planeCentre.A*planeCentre.A + planeCentre.B*planeCentre.B + planeCentre.C*planeCentre.C;	// коэффициент D - для плоскости centre

	/* TESTS CODE */

	/* default vector z {0;0;1} */
	vector_coord defaultZ;
	defaultZ.x = 0;
	defaultZ.y = 0;
	defaultZ.z = 1;

	/* отображение вектора defaultZ */
	double r = rootEquation(planeCentre, defaultZ);
//	qDebug() << "root = " << r;
	vector_coord vectorZ;
	vectorZ.x = centre.x*r + defaultZ.x;
	vectorZ.y = centre.y*r + defaultZ.y;
	vectorZ.z = centre.z*r + defaultZ.z;

//	vector_coord vectZ = subtractVector(vectorZ, centre);

	/* нахождение точки пересечения vect с Oz */
	vector_coord crossOz;
	crossOz.x = vectorZ.x;
	crossOz.y = vectorZ.y;
	crossOz.z = vectorZ.z;

//	crossOz.x = 0;
//	crossOz.y = 0;
//	crossOz.z = planeCentre.D / planeCentre.C;
//	qDebug() << "crossOz.z = " << crossOz.z;


	/* TESTS CODE */

	/* нахождение вектора направленного из centre в crossOz */
	vector_coord northVector = subtractVector(crossOz, centre);
//	qDebug() << "northVector.x = " << northVector.x;
//	qDebug() << "northVector.y = " << northVector.y;
//	qDebug() << "northVector.z = " << northVector.z;
	/* 2.Этап - проекция вектора target на плоскость vect */
	/* уравнение для нахождения координат ветора на плоскости */
	double t = rootEquation(planeCentre, target);
//	qDebug() << "root = " << t;
	vector_coord centreTarget;
	centreTarget.x = centre.x*t + target.x;
	centreTarget.y = centre.y*t + target.y;
	centreTarget.z = centre.z*t + target.z;

	vector_coord final = subtractVector(centreTarget, centre);
//	qDebug() << "final.x = " << final.x;
//	qDebug() << "final.y = " << final.y;
//	qDebug() << "final.z = " << final.z;


	/* Векторное произведение векторов - нормаль к двум векторам */
	vector_coord normCentreDefaultZ = vectorProduct(northVector, centre);
//	qDebug() << "normCentreDefaultZ.x = " << normCentreDefaultZ.x;
//	qDebug() << "normCentreDefaultZ.y = " << normCentreDefaultZ.y;
//	qDebug() << "normCentreDefaultZ.z = " << normCentreDefaultZ.z;

	/* нормировка до единичного */
	normCentreDefaultZ = vectorNorm(normCentreDefaultZ);

	/* вычисление угла между северным вектором и проекцией вектора target на плоскость central */
	double angleNorth = angleVectors(northVector, final);
//	qDebug() << "angleNorth = " << angleNorth*180/PI;
	double angleEast  = angleVectors(normCentreDefaultZ, final);
//	qDebug() << "angleEast = " << angleEast*180/PI;

	/* вычисление азимута */
	if(angleEast < rad90)
		return angleNorth;
	else
		return 2*PI - angleNorth;
}

int main(int argc, char *argv[])
{
//	TDataBase db;
	QCoreApplication a(argc, argv);

	QVector<int> vectInt;
	for(int i=0;i<10;i++){
		vectInt.append(i);
	}

	vectInt.remove(3);

//	vector_coord centre;
//	centre.x = 0;
//	centre.y = -20000;
//	centre.z = 0;

//	vector_coord target;
//	target.x = 546.715307;
//	target.y = -14317.826213;
//	target.z = 21093.251727;

////	double azimuth = azimuthCalc(centre, target);
////	qDebug() << azimuth*180/PI;
//	/* TEST */

//	/* test: 1 azimuth = 90 */
//	centre.x = 200;
//	centre.y = 0;
//	centre.z = 0;

//	target.x = 0;
//	target.y = 100;
//	target.z = 0;

//	qDebug() << "Test 1 azimuth(90) = " << azimuthCalc(centre, target)*180/PI;

//	/* test: 2 azimuth = 0 */
//	centre.x = 200;
//	centre.y = 0;
//	centre.z = 0;

//	target.x = 100;
//	target.y = 0;
//	target.z = 0;

//	qDebug() << "Test 2 azimuth(0) = " << azimuthCalc(centre, target)*180/PI;

//	/* test: 3 azimuth = 270 */
//	centre.x = 0;
//	centre.y = 100;
//	centre.z = 0;

//	target.x = 0;
//	target.y = 200;
//	target.z = 0;

//	qDebug() << "Test 3 azimuth(0) = " << azimuthCalc(centre, target)*180/PI;

//	/* test: 4 azimuth = 270 */
//	centre.x = 100;
//	centre.y = 0;
//	centre.z = 0;

//	target.x = 0;
//	target.y = -100;
//	target.z = 0;

//	qDebug() << "Test 4 azimuth(270) = " << azimuthCalc(centre, target)*180/PI;

//	/* test: 5 azimuth = 0 */
//	centre.x = 0;
//	centre.y = 120;
//	centre.z = 0;

//	target.x = 130;
//	target.y = 0;
//	target.z = 0;

//	qDebug() << "Test 5 azimuth(270) = " << azimuthCalc(centre, target)*180/PI;

//	/* test: 6 azimuth = 90 */
//	centre.x = 0;
//	centre.y = 100;
//	centre.z = 0;

//	target.x = -200;
//	target.y = 0;
//	target.z = 0;

//	qDebug() << "Test 6 azimuth(90) = " << azimuthCalc(centre, target)*180/PI;

//	double maxAngle = 360; // deg

//	centre.x = 0;
//	centre.y = 100;
//	centre.z = 0;

//	target.x = 4;
//	target.y = 30;
//	target.z = 10;
//	double  A = 100;
//	for(int i=0;i<maxAngle;i++){
//		double fi = i*PI/180;
//		double r = A/(2*PI)*fi;
//		target.x = r*cos(fi);
//		target.z = r*sin(fi);
////		qDebug() << r << target.x << target.z;
//		qDebug() << "Angle = " << azimuthCalc(centre, target)*180/PI;
//	}
//	qDebug() << "Angle = " << azimuthCalc(centre, target)*180/PI;

//	double azimuth = azimuthCalc2(centre, target);
//	qDebug() << "centre.x = " << centre.x
//			 << "centre.y = " << centre.y
//			 << "centre.z = " << centre.z;
//	qDebug() << "target.x = " << target.x
//			 << "target.y = " << target.y
//			 << "target.z = " << target.z;
//	qDebug() << "Azimuth" << azimuth*180/PI << "deg";

	return a.exec();
}
