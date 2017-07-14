#include "guass_newton.h"

int32_t InitMatrix(Matrix * M, uint32_t m, uint32_t n)
{
	M->m = m;
	M->n = n;
	M->Transposed = 0;
	M->data = (double **)malloc(m * sizeof(double *));
	if (M->data == NULL)
		return -1;
	for (uint32_t i = 0; i < m; i++)
	{
		M->data[i] = malloc(n * sizeof(double));
		if (M->data[m] == NULL)
			return -1;
	}
	
	return 0;
}

int32_t ReleaseMatrix(Matrix * M)
{
	//num2++;
	uint32_t m = M->m;
	for (uint32_t i = 0; i < m; i++)
	{
		free(M->data[i]);
	}
	free(M->data);
	return 0;
}

int32_t ShallowCopyMatrix(Matrix * Dest, Matrix * Src)
{
	Dest->m = Src->m;
	Dest->n = Src->n;
	Dest->data = Src->data;
	return 0;
}

int32_t DeepCopyMatrix(Matrix * Dest, Matrix * Src)
{
	uint32_t m = Row(Src);
	uint32_t n = Col(Src);
	for (uint32_t i = 0; i < m; i++)
	{
		for (uint32_t j = 0; j < n; j++)
		{
			*MatrixValue(Dest, i, j) = *MatrixValue(Src, i, j);
		}
	}
	return 0;

}

int32_t MatrixSetAll(Matrix * M, double value)
{
	uint32_t m = Row(M);
	uint32_t n = Col(M);
	for (uint32_t i = 0; i < m; i++)
	{
		for (uint32_t j = 0; j < n; j++)
		{
			*MatrixValue(M, i, j) = value;
		}
	}
	return 0;
}

int32_t MatrixTrans(Matrix * M, Matrix * TM)
{
	*TM = *M;
	TM->Transposed = !M->Transposed;
	return 0;
}

int32_t MatrixAdd(Matrix * M, Matrix * N)
{
	uint32_t m, n;
	if (Row(M) != Row(N) || Col(M) != Col(N))
		return -1;
	m = Row(M);
	n = Col(M);

	for (uint32_t i = 0; i < m; i++)
	{
		for (uint32_t j = 0; j < n; j++)
		{
			(*MatrixValue(M, i, j)) += *MatrixValue(N, i, j);
		}
	}
	return 0;
}

int32_t MatrixSub(Matrix * M, Matrix * N)
{
	uint32_t m, n;
	if (Row(M) != Row(N) || Col(M) != Col(N))
		return -1;
	m = Row(M);
	n = Col(M);

	for (uint32_t i = 0; i < m; i++)
	{
		for (uint32_t j = 0; j < n; j++)
		{
			(*MatrixValue(M, i, j)) -= *MatrixValue(N, i, j);
		}
	}
	return 0;
}

int32_t MatrixMult(Matrix * M, Matrix * N, Matrix * R)
{
	uint32_t m, n, p;
	if (Col(M) != Row(N))
		return -1;
	p = Col(M);

	m = Row(M);
	n = Col(N);
	
	double temp = 0.0;

	for (uint32_t i = 0; i < m; i++)
	{
		for (uint32_t j = 0; j < n; j++)
		{
			temp = 0.0;
			for (uint32_t k = 0; k < p; k++)
			{
				temp += (*MatrixValue(M, i, k)) * (*MatrixValue(N, k, j));
			}
			*MatrixValue(R, i, j) = temp;
		}
	}
	return 0;
}

int32_t MatrixMultScale(Matrix * M, double scale)
{
	uint32_t m, n;

	m = Row(M);
	n = Col(M);

	for (uint32_t i = 0; i < m; i++)
	{
		for (uint32_t j = 0; j < n; j++)
		{
			(*MatrixValue(M, i, j)) *= scale;
		}
	}
	return 0;
}

int32_t MatrixDivScale(Matrix * M, double scale)
{
	uint32_t m, n;

	m = Row(M);
	n = Col(M);

	for (uint32_t i = 0; i < m; i++)
	{
		for (uint32_t j = 0; j < n; j++)
		{
			(*MatrixValue(M, i, j)) /= scale;
		}
	}
	return 0;
}

int32_t MatrixFunc(e_func * F, Matrix * M, Matrix * P, Matrix * D)
{
	uint32_t m, n;
	m = Row(M);
	n = Col(M);

	for (uint32_t i = 0; i < m; i++)
	{
		for (uint32_t j = 0; j < n; j++)
		{
			*MatrixValue(M, i, j) = F[i * n + j](D->data, P->data);
		}
	}
	return 0;
}

double MatrixDet(Matrix * M)
{
	uint32_t i, j, j1, j2;
	double det = 0;
	uint32_t n = Row(M);
	Matrix m = { 0 };
	InitMatrix(&m, n - 1, n - 1);

	if (n < 1) { /* Error */

	}
	else if (n == 1) { /* Shouldn't get used */
		det = *MatrixValue(M, 0, 0);
	}
	else if (n == 2) {
		det = (*MatrixValue(M, 0, 0)) * (*MatrixValue(M, 1, 1)) - (*MatrixValue(M, 1, 0)) * (*MatrixValue(M, 0, 1));
	}
	else {
		det = 0;
		for (j1 = 0; j1<n; j1++) {
			
			for (i = 1; i<n; i++) {
				j2 = 0;
				for (j = 0; j<n; j++) {
					if (j == j1)
						continue;
					*MatrixValue(&m, i - 1, j2) = *MatrixValue(M, i, j);
					j2++;
				}
			}
			
			det += pow(-1.0, j1 + 2.0) * (*MatrixValue(M, 0, j1)) * MatrixDet(&m);
			
		}
	}

	ReleaseMatrix(&m);

	return(det);

}

int32_t MatrixCoFactor(Matrix * M, Matrix * R)
{
	uint32_t i, j, ii, jj, i1, j1;
	double det;
	uint32_t n = Row(M);
	Matrix c = { 0 };
	InitMatrix(&c ,n - 1, n - 1);


	for (j = 0; j<n; j++) {
		for (i = 0; i<n; i++) {

			/* Form the adjoint a_ij */
			i1 = 0;
			for (ii = 0; ii<n; ii++) {
				if (ii == i)
					continue;
				j1 = 0;
				for (jj = 0; jj<n; jj++) {
					if (jj == j)
						continue;
					*MatrixValue(&c, i1, j1) = *MatrixValue(M, ii, jj);
					j1++;
				}
				i1++;
			}

			/* Calculate the determinate */
			det = MatrixDet(&c);

			/* Fill in the elements of the cofactor */
			*MatrixValue(R, i, j) = pow(-1.0, i + j + 2.0) * det;
		}
	}

	ReleaseMatrix(&c);
	return 0;
}

int32_t MatrixInverse(Matrix * M, Matrix * R)
{
	double det = MatrixDet(M);
	MatrixCoFactor(M, R);
	MatrixTrans(R, R);
	MatrixDivScale(R, det);
	return 0;
}

double VectorNorm2(Matrix * v)
{
	Matrix r = { 0 };
	double norm2 = 0.0;
	InitMatrix(&r, 1, 1);
	Matrix t = { 0 };
	MatrixTrans(v, &t);

	MatrixMult(&t, v, &r);
	
	norm2 = *MatrixValue(&r, 0, 0);

	return norm2;
}

int32_t MatrixSwitch(Matrix * M, Matrix * N)
{
	Matrix temp = { 0 };
	temp = *M;
	*M = *N;
	*N = temp;
	return 0;
}

int32_t MatrixLUDecomposition(Matrix * M, Matrix * L, Matrix * U, Matrix * P)
{
	return 0;
}

int32_t MatrixSolverGE(Matrix * M, Matrix * B)
{
	uint32_t n = M->n;
	uint32_t m = Col(B);

	double temp1, temp2, a;
	for (uint32_t i = 0; i < n; i++)
	{
		temp1 = *MatrixValue(M, i, i);
		if (IS_ZERO(temp1))
		{
			char erro_flag = 0;
			for (uint32_t k = n - 1; k > i; k--)
			{
				if (IS_NOT_ZERO(*MatrixValue(M, k, i)))
				{
					double * temp_ptr = NULL;
					temp_ptr = M->data[i];
					M->data[i] = M->data[k];
					M->data[k] = temp_ptr;
					temp_ptr = B->data[i];
					B->data[i] = B->data[k];
					B->data[k] = temp_ptr;
					temp1 = *MatrixValue(M, i, i);
					erro_flag = 0;
					break;
				}
			}
			if (!erro_flag)
				return -1;
		}

		if (i == n - 1)
			break;

		for (uint32_t j = i + 1; j < n; j++)
		{
			temp2 = *MatrixValue(M, j, i);
			if (IS_ZERO(temp2))
			{
				*MatrixValue(M, j, i) = 0.0;
				continue;
			}
			a = temp2 / temp1;
			*MatrixValue(M, j, i) = 0.0;
			for (uint32_t k = i + 1; k < n; k++)
			{
				*MatrixValue(M, j, k) -= *MatrixValue(M, i, k) * a;

			}
			for (uint32_t k = 0; k < m; k++)
			{
				*MatrixValue(B, j, k) -= *MatrixValue(B, i, k) * a;
			}

		}
	}

	for (uint32_t i = n - 1; i > 0; i--)
	{
		temp1 = *MatrixValue(M, i, i);
		for (int32_t j = i - 1; j >= 0; j--)
		{
			temp2 = *MatrixValue(M, j, i);
			if (IS_ZERO(temp2))
				continue;
			a = temp2 / temp1;
			*MatrixValue(M, j, i) = 0.0;
			for (uint32_t k = 0; k < m; k++)
			{
				*MatrixValue(B, j, k) -= *MatrixValue(B, i, k) * a;
			}

		}
	}
	for (uint32_t i = 0; i < n; i++)
	{
		temp1 = *MatrixValue(M, i, i);
		for (uint32_t j = 0; j < m; j++)
		{
			*MatrixValue(B, i, j) /= temp1;
		}
	}
	return 0;
}

int32_t MatrixInverseGE(Matrix * M, Matrix * R, uint8_t opt)
{
	Matrix E = { 0 };

	//InitMatrix(&E, M->m, M->n);
	if (R->Transposed)
		R->Transposed = 0;

	if (opt == MATRIX_NO_COPY)
	{
		if (M->Transposed)
		{
			Matrix temp = { 0 };
			InitMatrix(&temp, Row(M), Col(M));
			DeepCopyMatrix(&temp, M);
			MatrixSwitch(&temp, M);
			ReleaseMatrix(&temp);
		}
		ShallowCopyMatrix(&E, M);
	}
	else
	{
		InitMatrix(&E, Row(M), Col(M));
		DeepCopyMatrix(&E, M);
	}
	
	MatrixSetAll(R, 0.0);

	uint32_t n = M->n;

	for (uint32_t i = 0; i < n; i++)
	{
		*MatrixValue(R, i, i) = 1.0;
	}

	if( MatrixSolverGE(&E, R) == -1 )
		return -1;

	/*

	

	
	double temp1, temp2, a;
	for (uint32_t i = 0; i < n; i++)
	{
		temp1 = *MatrixValue(&E, i, i);
		if (IS_ZERO(temp1))
		{
			char erro_flag = 0;
			for (uint32_t k = n - 1; k > i; k--)
			{
				if (IS_NOT_ZERO(*MatrixValue(&E, k, i)))
				{
					double * temp_ptr = NULL;
					temp_ptr = E.data[i];
					E.data[i] = E.data[k];
					E.data[k] = temp_ptr;
					temp_ptr = R->data[i];
					R->data[i] = R->data[k];
					R->data[k] = temp_ptr;
					temp1 = *MatrixValue(M, i, i);
					erro_flag = 0;
					break;
				}
			}
			if (!erro_flag)
				return -1;
		}
		
		if (i == n - 1)
			break;

		for (uint32_t j = i + 1; j < n; j++)
		{
			temp2 = *MatrixValue(&E, j, i);
			if (IS_ZERO(temp2))
			{
				*MatrixValue(&E, j, i) = 0.0;
				continue;
			}
			a = temp2 / temp1;
			*MatrixValue(&E, j, i) = 0.0;
			for (uint32_t k = i + 1; k < n; k++)
			{
				*MatrixValue(&E, j, k) -= *MatrixValue(&E, i, k) * a;

			}
			for (uint32_t k = 0; k < n; k++)
			{
				*MatrixValue(R, j, k) -= *MatrixValue(R, i, k) * a;
			}

		}
	}

	for (uint32_t i = n - 1; i > 0; i--)
	{
		temp1 = *MatrixValue(&E, i, i);
		for (int32_t j = i - 1; j >= 0; j--)
		{
			temp2 = *MatrixValue(&E, j, i);
			if (IS_ZERO(temp2))
				continue;
			a = temp2 / temp1;
			*MatrixValue(&E, j, i) = 0.0;
			for (uint32_t k = 0; k < n; k++)
			{
				*MatrixValue(R, j, k) -= *MatrixValue(R, i, k) * a;
			}

		}
	}
	for (uint32_t i = 0; i < n; i++)
	{
		temp1 = *MatrixValue(&E, i, i);
		for (uint32_t j = 0; j < n; j++)
		{
			*MatrixValue(R, i, j) /= temp1;
		}
	}
	*/
	if (opt == MATRIX_COPY)
	{
		ReleaseMatrix(&E);
	}
	return 0;
}

void guass_newton(e_func E, e_func * J, e_func * H, uint32_t p_dim, double * data, uint32_t data_dim, uint32_t data_N, double * p)
{
	Matrix Hessian = { 0 };
	InitMatrix(&Hessian, p_dim, p_dim);

	Matrix Jacobi = { 0 };
	InitMatrix(&Jacobi, p_dim, 1);

	Matrix E_Diff = { 0 };
	Matrix E_Diff_T = { 0 };
	InitMatrix(&E_Diff, p_dim, 1);

	Matrix MulResult = { 0 };
	InitMatrix(&MulResult, p_dim, p_dim);

	Matrix Param = { 0 };
	InitMatrix(&Param, p_dim, 1);

//	Matrix Param_per = { 0 };
//	InitMatrix(&Param_per, p_dim, 1);

	Matrix inv = { 0 };
	InitMatrix(&inv, p_dim, p_dim);

	Matrix step_vector = { 0 };
	InitMatrix(&step_vector, p_dim, 1);

	for (uint32_t i = 0; i < p_dim; i++)
	{
		*MatrixValue(&Param, i, 0) = p[i];
	}

	Matrix Data = { 0 };
	InitMatrix(&Data, data_dim, 1);

	double a = 0.0;
	double b = 0.0;


	while (1)
	{

		MatrixSetAll(&Hessian, 0.0);
		MatrixSetAll(&Jacobi, 0.0);

		for (uint32_t i = 0; i < data_N; i++)
		{
			double * data_ptr = data + i * data_dim;
			*MatrixValue(&Data, 0, 0) = data_ptr[0];
			*MatrixValue(&Data, 1, 0) = data_ptr[1];
			*MatrixValue(&Data, 2, 0) = data_ptr[2];



			MatrixFunc(J, &E_Diff, &Param, &Data);
			MatrixTrans(&E_Diff, &E_Diff_T);
			MatrixMult(&E_Diff, &E_Diff_T, &MulResult);
			MatrixAdd(&Hessian, &MulResult);

			MatrixMultScale(&E_Diff, E(Data.data, Param.data));
			MatrixAdd(&Jacobi, &E_Diff);
			
		}

		MatrixSolverGE(&Hessian, &Jacobi);

		MatrixSub(&Param, &Jacobi);

		a = VectorNorm2(&Jacobi);

		b = VectorNorm2(&Param);

		if (a / b < 1.0e-18)
			break;

	}


	for (uint32_t i = 0; i < p_dim; i++)
	{
		p[i] = *MatrixValue(&Param, i, 0);
		//printf("%f\n", p[i]);
	}


	ReleaseMatrix(&Data);

	ReleaseMatrix(&Hessian);

	ReleaseMatrix(&Jacobi);

	ReleaseMatrix(&E_Diff);

	ReleaseMatrix(&MulResult);

	ReleaseMatrix(&Param);

//	ReleaseMatrix(&Param_per);

	ReleaseMatrix(&inv);

	ReleaseMatrix(&step_vector);

}
