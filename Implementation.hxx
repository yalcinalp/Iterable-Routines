#pragma once

#include "Iterable.hxx"
#include <vector>

template<typename T>
class RepeatIterator : public IIterator<T>
{
private:
    T value;

public:
    RepeatIterator(T v) : value(v) {}

    T next() override
    {
        // Always return the value, since it's an infinite sequence
        return value;
    }
};

template<typename T>
class RepeatIterable : public IIterable<T>
{
private:
    T value;

public:
    RepeatIterable(T v) : value(v) {}

    IIterator<T>* iter() const override
    {
        return new RepeatIterator<T>(value);
    }
};

template<typename T>
IIterable<T>* repeat(T v)
{
    return new RepeatIterable<T>(v);
}


template<typename T>
class CountIterator : public IIterator<T>
{
private:
    T currentValue;
    T step;

public:
    CountIterator(T start, T delta) : currentValue(start), step(delta) {}

    T next() override
    {
        // Return the current value and update it by adding step
        T returnValue = currentValue;
        currentValue += step;
        return returnValue;
    }
};

template<typename T>
class CountIterable : public IIterable<T>
{
private:
    T startValue;
    T stepValue;

public:
    CountIterable(T start, T delta) : startValue(start), stepValue(delta) {}

    IIterator<T>* iter() const override
    {
        return new CountIterator<T>(startValue, stepValue);
    }
};

template<typename T>
IIterable<T>* count(T start, T delta)
{
    return new CountIterable<T>(start, delta);
}

template<typename T>
class SkipIterator : public IIterator<T>
{
private:
    IIterator<T>* sourceIterator;
    unsigned skipCount;

public:
    SkipIterator(unsigned k, IIterator<T>* it) : sourceIterator(it), skipCount(k)
    {
        // Skip the first k elements
        try {
            for (unsigned int j = 0; j < k; ++j) {
                sourceIterator->next();
            }
        } catch (const StopIteration&) {
            // If we run out of elements while skipping, subsequent calls to next() should also throw.
        }
    }

    T next() override
    {
        return sourceIterator->next();
    }

    ~SkipIterator()
    {
        delete sourceIterator;
    }
};

template<typename T>
class SkipIterable : public IIterable<T>
{
private:
    IIterable<T>* sourceIterable;
    unsigned skipCount;

public:
    SkipIterable(unsigned k, IIterable<T>* iterable) : sourceIterable(iterable), skipCount(k) {}

    IIterator<T>* iter() const override
    {
        return new SkipIterator<T>(skipCount, sourceIterable->iter());
    }

    ~SkipIterable()
    {
        delete sourceIterable;
    }
};

template<typename T>
IIterable<T>* skip(unsigned k, IIterable<T>* i)
{
    return new SkipIterable<T>(k, i);
}


template<typename T>
class TakeIterator : public IIterator<T>
{
private:
    IIterator<T>* sourceIterator;
    unsigned remaining;

public:
    TakeIterator(unsigned k, IIterator<T>* it) : sourceIterator(it), remaining(k) {}

    T next() override
    {
        if (remaining == 0) {
            throw StopIteration();
        }
        T value = sourceIterator->next();
        remaining--;
        return value;
    }

    ~TakeIterator() {
        delete sourceIterator;
    }
};

template<typename T>
class TakeIterable : public IIterable<T>
{
private:
    IIterable<T>* sourceIterable;
    unsigned count;

public:
    TakeIterable(unsigned k, IIterable<T>* iterable) : sourceIterable(iterable), count(k) {}

    IIterator<T>* iter() const override
    {
        return new TakeIterator<T>(count, sourceIterable->iter());
    }

    ~TakeIterable() {
        delete sourceIterable;
    }
};

template<typename T>
IIterable<T>* take(unsigned k, IIterable<T>* i)
{
    return new TakeIterable<T>(k, i);
}

template<typename T>
class AlternateIterator : public IIterator<T>
{
private:
    IIterator<T>* iteratorI;
    IIterator<T>* iteratorJ;
    bool turn;  // false for i, true for j

public:
    AlternateIterator(IIterator<T>* itI, IIterator<T>* itJ) : iteratorI(itI), iteratorJ(itJ), turn(false) {}

    T next() override
    {
        if (!turn) {
            turn = !turn;
            try {
                return iteratorI->next();
            } catch (StopIteration&) {
                
                turn = true;
                return iteratorJ->next(); 
            }
        } else { 
            turn = !turn;
            try {
                return iteratorJ->next();
            } catch (StopIteration&) {
                
                turn = false;
                return iteratorI->next(); 
            }
        }
    }

    ~AlternateIterator() {
        delete iteratorI;
        delete iteratorJ;
    }
};

template<typename T>
class AlternateIterable : public IIterable<T>
{
private:
    IIterable<T>* iterableI;
    IIterable<T>* iterableJ;

public:
    AlternateIterable(IIterable<T>* i, IIterable<T>* j) : iterableI(i), iterableJ(j) {}

    IIterator<T>* iter() const override
    {
        return new AlternateIterator<T>(iterableI->iter(), iterableJ->iter());
    }

    ~AlternateIterable() {
        delete iterableI;
        delete iterableJ;
    }
};

template<typename T>
IIterable<T>* alternate(IIterable<T>* i, IIterable<T>* j)
{
    return new AlternateIterable<T>(i, j);
}

template<typename T>
class CycleIterator : public IIterator<T>
{
private:
    IIterable<T>* sourceIterable;
    IIterator<T>* currentIterator;
    bool hasElements;

public:
    CycleIterator(IIterable<T>* iterable) : sourceIterable(iterable), currentIterator(nullptr), hasElements(false)
    {
        restartIterator();
        try {
            currentIterator->next();
            hasElements = true;
            restartIterator();
        } catch (StopIteration&) {
            hasElements = false;
        }
    }

    void restartIterator()
    {
        if (currentIterator) {
            delete currentIterator;
        }
        currentIterator = sourceIterable->iter();
    }

    T next() override
    {
        if (!hasElements) {
            throw StopIteration();
        }

        try {
            return currentIterator->next();
        } catch (StopIteration&) {
            restartIterator();
            return currentIterator->next();
        }
    }

    ~CycleIterator()
    {
        delete currentIterator;
    }
};

template<typename T>
class CycleIterable : public IIterable<T>
{
private:
    IIterable<T>* sourceIterable;

public:
    CycleIterable(IIterable<T>* iterable) : sourceIterable(iterable) {}

    IIterator<T>* iter() const override
    {
        return new CycleIterator<T>(sourceIterable);
    }

    ~CycleIterable()
    {
        delete sourceIterable;
    }
};

template<typename T>
IIterable<T>* cycle(IIterable<T>* i)
{
    return new CycleIterable<T>(i);
}

template<typename T>
class FlattenIterator : public IIterator<T>
{
private:
    IIterable<IIterable<T>*>* sourceIterable;
    IIterator<IIterable<T>*>* outerIterator;
    IIterator<T>* innerIterator;

public:
    FlattenIterator(IIterable<IIterable<T>*>* iterable) : sourceIterable(iterable), outerIterator(nullptr), innerIterator(nullptr)
    {
        outerIterator = sourceIterable->iter();
        innerIterator = nullptr;
    }

    T next() override
    {
        while (true)
        {
            if (innerIterator != nullptr)
            {
                try
                {
                    return innerIterator->next();
                }
                catch (StopIteration&)
                {
                    delete innerIterator;
                    innerIterator = nullptr;
                }
            }

            IIterable<T>* innerIterable = outerIterator->next();
            innerIterator = innerIterable->iter();
        }
    }

    ~FlattenIterator()
    {
        delete outerIterator;
        delete innerIterator;
    }
};


template<typename T>
class FlattenIterable : public IIterable<T>
{
private:
    IIterable<IIterable<T>*>* sourceIterable;

public:
    FlattenIterable(IIterable<IIterable<T>*>* iterable) : sourceIterable(iterable) {}

    IIterator<T>* iter() const override
    {
        return new FlattenIterator<T>(sourceIterable);
    }

    ~FlattenIterable()
    {
        delete sourceIterable;
    }
};

template<typename T>
IIterable<T>* flatten(IIterable<IIterable<T>*>* i)
{
    return new FlattenIterable<T>(i);
}

class MultabIterator : public IIterator<IIterable<T>*>
{
private:
    IIterable<T>* iterableI;
    IIterable<T>* iterableJ;
    IIterator<T>* iteratorI;
    IIterator<T>* iteratorJ;

public:
    MultabIterator(IIterable<T>* i, IIterable<T>* j) : iterableI(i), iterableJ(j), iteratorI(nullptr), iteratorJ(nullptr)

    {
        iteratorI = iterableI->iter();
        iteratorJ = iterableJ->iter();
    }
    IIterable<T>* next() override
    {
        if (iteratorI == nullptr || iteratorJ == nullptr) {
            throw StopIteration();
        }

        std::vector<T> row;
        try {
            while (true) {
                T valueI = iteratorI->next();
                iteratorJ->iter();
                while (true) {
                    try {
                        T valueJ = iteratorJ->next();
                        row.push_back(valueI * valueJ);
                    } catch (StopIteration&) {
                        break;
                    }
                }
            }
        } catch (StopIteration&) {
            if (row.empty()) {
                throw StopIteration();
            }
            return new RepeatIterable<T>(row);
        }
    }
    ~MultabIterator()
    {
        delete iteratorI;
        delete iteratorJ;
    }
};

template<typename T>
class MultabIterable : public IIterable<IIterable<T>*>
{
private:
    IIterable<T>* iterableI;
    IIterable<T>* iterableJ;

public:
    MultabIterable(IIterable<T>* i, IIterable<T>* j) : iterableI(i), iterableJ(j) {}

    IIterator<IIterable<T>*>* iter() const override
    {
        return new MultabIterator<T>(iterableI, iterableJ);
    }
    ~MultabIterable()
    {
        delete iterableI;
        delete iterableJ;
    }
};

template<typename T>
IIterable<IIterable<T>*>* multab(IIterable<T>* i, IIterable<T>* j)
{
    return new MultabIterable<T>(i, j);
}
