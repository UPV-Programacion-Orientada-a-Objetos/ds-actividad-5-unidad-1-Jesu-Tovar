#include <iostream>

template <typename T>
class matrizbase {
protected:
    int filas;
    int columnas;

public:
    matrizbase(int f, int c) : filas(f), columnas(c) {}
    virtual ~matrizbase() {}
    
    virtual void llenarvalores() = 0;
    virtual matrizbase<T>* sumar(const matrizbase<T>& otra) const = 0;
    virtual void mostrar() const = 0;
    
    int getfilas() const { return filas; }
    int getcolumnas() const { return columnas; }
};

template <typename T>
class matrizdinamica : public matrizbase<T> {
private:
    T** datos;
    
    void crearmemoria() {
        datos = new T*[this->filas];
        for (int pepito = 0; pepito < this->filas; pepito++) {
            datos[pepito] = new T[this->columnas];
        }
    }
    
    void liberarmemoria() {
        if (datos) {
            for (int pepito = 0; pepito < this->filas; pepito++) {
                delete[] datos[pepito];
            }
            delete[] datos;
            datos = nullptr;
        }
    }

public:
    matrizdinamica(int f, int c) : matrizbase<T>(f, c), datos(nullptr) {
        crearmemoria();
        for (int pepito = 0; pepito < this->filas; pepito++) {
            for (int negro = 0; negro < this->columnas; negro++) {
                datos[pepito][negro] = T(0);
            }
        }
    }
    
    ~matrizdinamica() {
        std::cout << "Liberando memoria de Matriz Dinamica..." << std::endl;
        liberarmemoria();
    }
    
    matrizdinamica(const matrizdinamica<T>& otra) : matrizbase<T>(otra.filas, otra.columnas) {
        crearmemoria();
        for (int pepito = 0; pepito < this->filas; pepito++) {
            for (int negro = 0; negro < this->columnas; negro++) {
                datos[pepito][negro] = otra.datos[pepito][negro];
            }
        }
    }
    
    matrizdinamica<T>& operator=(const matrizdinamica<T>& otra) {
        if (this != &otra) {
            liberarmemoria();
            this->filas = otra.filas;
            this->columnas = otra.columnas;
            crearmemoria();
            for (int pepito = 0; pepito < this->filas; pepito++) {
                for (int negro = 0; negro < this->columnas; negro++) {
                    datos[pepito][negro] = otra.datos[pepito][negro];
                }
            }
        }
        return *this;
    }
    
    void llenarvalores() override {
        std::cout << "Ingresa valores para matriz " << this->filas << "x" << this->columnas << ":" << std::endl;
        for (int pepito = 0; pepito < this->filas; pepito++) {
            for (int negro = 0; negro < this->columnas; negro++) {
                std::cout << "Posicion [" << pepito << "][" << negro << "]: ";
                std::cin >> datos[pepito][negro];
            }
        }
    }
    
    matrizbase<T>* sumar(const matrizbase<T>& otra) const override {
        if (this->filas != otra.getfilas() || this->columnas != otra.getcolumnas()) {
            std::cout << "No se pueden sumar, dimensiones diferentes!" << std::endl;
            return nullptr;
        }
        
        matrizdinamica<T>* resultado = new matrizdinamica<T>(this->filas, this->columnas);
        
        const matrizdinamica<T>* otradinamica = dynamic_cast<const matrizdinamica<T>*>(&otra);
        
        for (int pepito = 0; pepito < this->filas; pepito++) {
            for (int negro = 0; negro < this->columnas; negro++) {
                resultado->datos[pepito][negro] = this->datos[pepito][negro] + otradinamica->datos[pepito][negro];
            }
        }
        
        return resultado;
    }
    
    matrizdinamica<T> operator+(const matrizdinamica<T>& otra) const {
        matrizdinamica<T> resultado(this->filas, this->columnas);
        for (int pepito = 0; pepito < this->filas; pepito++) {
            for (int negro = 0; negro < this->columnas; negro++) {
                resultado.datos[pepito][negro] = this->datos[pepito][negro] + otra.datos[pepito][negro];
            }
        }
        return resultado;
    }
    
    void mostrar() const override {
        for (int pepito = 0; pepito < this->filas; pepito++) {
            std::cout << "| ";
            for (int negro = 0; negro < this->columnas; negro++) {
                std::cout << datos[pepito][negro] << " | ";
            }
            std::cout << std::endl;
        }
    }
    
    void setvalor(int f, int c, T valor) {
        if (f >= 0 && f < this->filas && c >= 0 && c < this->columnas) {
            datos[f][c] = valor;
        }
    }
};

template <typename T, int M, int N>
class matrizestatica : public matrizbase<T> {
private:
    T datos[M][N];

public:
    matrizestatica() : matrizbase<T>(M, N) {
        for (int pepito = 0; pepito < M; pepito++) {
            for (int negro = 0; negro < N; negro++) {
                datos[pepito][negro] = T(0);
            }
        }
    }
    
    void llenarvalores() override {
        std::cout << "Ingresa valores para matriz estatica " << M << "x" << N << ":" << std::endl;
        for (int pepito = 0; pepito < M; pepito++) {
            for (int negro = 0; negro < N; negro++) {
                std::cout << "Posicion [" << pepito << "][" << negro << "]: ";
                std::cin >> datos[pepito][negro];
            }
        }
    }
    
    matrizbase<T>* sumar(const matrizbase<T>& otra) const override {
        if (this->filas != otra.getfilas() || this->columnas != otra.getcolumnas()) {
            std::cout << "No se pueden sumar, dimensiones diferentes!" << std::endl;
            return nullptr;
        }
        
        matrizdinamica<T>* resultado = new matrizdinamica<T>(M, N);
        
        const matrizestatica<T, M, N>* otraestatica = dynamic_cast<const matrizestatica<T, M, N>*>(&otra);
        
        for (int pepito = 0; pepito < M; pepito++) {
            for (int negro = 0; negro < N; negro++) {
                T valorresultado = this->datos[pepito][negro] + otraestatica->datos[pepito][negro];
                resultado->setvalor(pepito, negro, valorresultado);
            }
        }
        
        return resultado;
    }
    
    void mostrar() const override {
        for (int pepito = 0; pepito < M; pepito++) {
            std::cout << "| ";
            for (int negro = 0; negro < N; negro++) {
                std::cout << datos[pepito][negro] << " | ";
            }
            std::cout << std::endl;
        }
    }
    
    void setvalor(int f, int c, T valor) {
        if (f >= 0 && f < M && c >= 0 && c < N) {
            datos[f][c] = valor;
        }
    }
};

int main() {
    std::cout << "--- Sistema generico de Algebra Lineal ---\n" << std::endl;
    std::cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<\n" << std::endl;
    
    std::cout << "Creando Matriz Dinamica A (3x2)..." << std::endl;
    matrizbase<float>* A = new matrizdinamica<float>(3, 2);
    
    dynamic_cast<matrizdinamica<float>*>(A)->setvalor(0, 0, 1.5);
    dynamic_cast<matrizdinamica<float>*>(A)->setvalor(0, 1, 2.0);
    dynamic_cast<matrizdinamica<float>*>(A)->setvalor(1, 0, 0.0);
    dynamic_cast<matrizdinamica<float>*>(A)->setvalor(1, 1, 1.0);
    dynamic_cast<matrizdinamica<float>*>(A)->setvalor(2, 0, 4.5);
    dynamic_cast<matrizdinamica<float>*>(A)->setvalor(2, 1, 3.0);
    
    std::cout << "A =" << std::endl;
    A->mostrar();
    std::cout << std::endl;
    
    std::cout << "Creando Matriz Estatica B (3x2)..." << std::endl;
    matrizbase<float>* B = new matrizestatica<float, 3, 2>();
    
    dynamic_cast<matrizestatica<float, 3, 2>*>(B)->setvalor(0, 0, 0.5);
    dynamic_cast<matrizestatica<float, 3, 2>*>(B)->setvalor(0, 1, 1.0);
    dynamic_cast<matrizestatica<float, 3, 2>*>(B)->setvalor(1, 0, 2.0);
    dynamic_cast<matrizestatica<float, 3, 2>*>(B)->setvalor(1, 1, 3.0);
    dynamic_cast<matrizestatica<float, 3, 2>*>(B)->setvalor(2, 0, 1.0);
    dynamic_cast<matrizestatica<float, 3, 2>*>(B)->setvalor(2, 1, 1.0);
    
    std::cout << "B =" << std::endl;
    B->mostrar();
    std::cout << std::endl;
    
    std::cout << "SUMANDO: Matriz C = A + B ..." << std::endl;
    matrizbase<float>* C = A->sumar(*B);
    
    std::cout << "\nMatriz Resultado C (3x2, Tipo FLOAT):" << std::endl;
    C->mostrar();
    
    std::cout << "\n>> Demostracion de Limpieza de Memoria <<" << std::endl;
    std::cout << "Llamando al destructor de C..." << std::endl;
    delete C;
    std::cout << "Llamando al destructor de A..." << std::endl;
    delete A;
    std::cout << "Llamando al destructor de B..." << std::endl;
    delete B;
    
    std::cout << "Sistema cerrado." << std::endl;
    
    return 0;
}