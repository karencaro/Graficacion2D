#include "StdAfx.h"
#include "Dibujo.h"

CDibujo::CDibujo(void)
{
}

CDibujo::~CDibujo(void)
{
}

void CDibujo::AddPolygon(CPoligono poli)
{
	m_figuras.push_back(poli);
}

CPoligono & CDibujo::operator[](unsigned int index)
{
	//TODO: insert return statement here
	if(index>=m_figuras.size())
		return m_figuras[0];
	return m_figuras[index];
}

int CDibujo::Save(CString filename)
{
	FILE *output;
	output=fopen(filename.GetBuffer(),"w");
	if(!output)
		return 0;
	fprintf(output,"%d\n",m_figuras.size()); //Numero de figuras en el dibujo
	for(int i=0;i<m_figuras.size();i++){
		m_figuras[i].Save(output);   //Cada figura se graba
	}
	fclose(output);
	return 1;
}

int CDibujo::Load(CString filename)
{
	FILE *input;
	int num_figuras=0;
	input=fopen(filename.GetBuffer(),"r");
	if(!input)
		return 0;
	m_figuras.clear();
	fscanf(input,"%d\n",&num_figuras); //Lee el numero de figuras en el dibujo
	for(int i=0;i<num_figuras;i++){
		CPoligono poli;
		poli.Load(input);   //Carga cada una de las figuras y las inserta en el dibujo
		m_figuras.push_back(poli);
	}
	fclose(input);
	return 1;
}
