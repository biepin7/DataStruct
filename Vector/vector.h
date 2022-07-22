//
// Created by 02 on 2022/6/10.
//

#ifndef DATASTRUCT_VECTOR_H
#define DATASTRUCT_VECTOR_H

#define DEFAULT_CAPACITY  3 //默认的初始容量

using Rank = int; // 秩,用 using 取别名，别问为什么，就是这么写的

template <typename T> class Vector{
private:
    Rank _size;  //规模
    Rank _capacity;  //容量
    T* _elem; //数据区
protected:
    /** 内部函数 */

    void copyFrom ( T const* A, Rank lo, Rank hi ) { //复制数组区间A[lo, hi) O(n)
        _elem = new T[_capacity = 2 * (hi -lo)];
        _size = 0;
        while (lo<hi) _elem[_size++] = A[lo++];
    }

    void expand(){ //空间不足时扩容
        if (_size < _capacity) return; //没满，无需扩容
        if(_capacity < DEFAULT_CAPACITY ) _capacity = DEFAULT_CAPACITY; // 注意不能小于规定的最小容量
        T* oldElem = _elem; // 保存旧容量用以复制原向量内容
        _elem = new T[_capacity <<= 1]; // 容量翻倍
        for (Rank i=0;i<_size;i++){
            _elem[i] = oldElem[i]; //复制原向量内容（T为基本类型，或已重载赋值操作符'='）
        }
        delete [] oldElem; //释放原空间
    }


public:
    /** 构造函数 */
    Vector(int c = DEFAULT_CAPACITY){ //默认构造函数
        _elem = new T[_capacity = c];
        size = 0;
    }

    Vector ( int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0 ){ //容量为c、规模为s、所有元素初始为v
        _elem = new T[_capacity = c];
        for ( _size = 0; _size < s; _elem[_size++] = v );
    } //s<=c

    Vector ( T const* A, Rank n ) { //数组整体复制
        copyFrom ( A, 0, n );
    }

    Vector(T const *A,Rank lo,Rank hi){ // 数组区间复制
        copyFrom(A,lo,hi);
    }

    Vector(Vector<T> const& V,Rank lo,Rank hi){ // 向量区间复制
        copyFrom(V._elem,lo,hi);
    }

    Vector(Vector<T> V){ // 向量整体复制
        copyFrom(V._elem,0,V._size);
    }

    /** 析构函数 */
    ~Vector() { delete [] _elem; }


    /** 只读接口 */

    /** 可写接口 */
    template <typename T> T & Vector<T>::operator[] ( Rank r ) {//重载下标操作符
        return _elem[r];  // assert: 0 <= r < _size
    }

    Rank insert(Rank r,T const& e){ // 将 e 作为秩为 r 的元素插入
        expand();
        for ( Rank i = _size ; r < i ; i --){ //自后向前移动元素后移
            _elem[i] = _elem[i-1];
        }
        _elem[r] = e ;
        size++;
        return r;
    }

    int remove(Rank lo,Rank hi) { //删除区间 [lo,hi) 内的元素
        if (lo == hi) return 0;
        while (hi < _size)
            _elem[lo++] = _elem[hi++];
        _size = lo;
        return hi -lo ;
    }

    T remove(Rank r){ // [r] == [r,r+1)
        T e = _elem[r];
        remove(r,r+1);
        return e;
    }

    Rank find(T const& e, Rank lo,Rank hi) const{ //无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
        while ((lo<hi--) && (e != _elem[hi]));//从后向前，顺序查找
        return  hi;//若hi < lo，则意味着失败；否则hi即命中元素的秩
    }
    // PS : 我觉得这么写是不是有点问题

    /** 遍历接口 */

    int disordered() const{ //返回相邻逆序对的总数，若判断是否有序，n>0 立马终止
        int n = 0 ;//逆序对数，初始为0
        for (int i = 1 ;i<_size;i++){ // 逐一检查各对相邻元素
            n += (_elem[i-1] > _elem[i]); //逆序计数
        }
        return n;
    }
};


#endif //DATASTRUCT_VECTOR_H
