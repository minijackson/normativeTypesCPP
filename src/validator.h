/* validator.h */
/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <set>

#include <pv/pvIntrospect.h>


namespace epics { namespace nt {

/**
 * @brief Validation methods for NT types.
 *
 * @author bsm
 */

struct Result {
    struct Error {
        std::string path;
        enum Type {
            MissingField,
            IncorrectType,
            IncorrectId,
        } type;

        Error(std::string const & path, Type type)
        : path(path), type(type) {}

        bool operator==(const Error& other) const {
            return type == other.type && path == other.path;
        }

        std::ostream& dump(std::ostream& os) const {
            os << "Error(path=" << (path.empty() ? "<root>" : path) << ": ";

            switch(type) {
                case MissingField:  os << "missing";        break;
                case IncorrectType: os << "incorrect type"; break;
                case IncorrectId:   os << "incorrect ID";   break;
            }
            os << ")";
            return os;
        }
    };

    const epics::pvData::FieldConstPtr f;
    const std::string path;
    std::vector<Error> errors;

    enum result_t {
        Pass,
        Fail,
    } result;

    explicit Result(const epics::pvData::FieldConstPtr& f, const std::string& path = std::string())
    : f(f), path(path), errors(), result(Pass) {}

    Result& operator|=(const Result& other) {
        result = std::max(result, other.result);
        errors.insert(errors.end(), other.errors.begin(), other.errors.end());
        return *this;
    }

    /**
     * Returns whether this Result is valid.
     *
     * @return true if all tests passed, false otherwise.
     */
    bool valid(void) const {
        return result == Pass;
    }

    /**
     * Test that this Result's field is of a particular type 'T'.
     *
     * Appends an Error::Type::IncorrectType if the field is not of type 'T'.
     *
     * @return itself
     */
    template<typename T>
    Result& is(void) {
        if (!dynamic_cast<T const *>(f.get())) {
            result = Fail;
            errors.push_back(Error(path, Error::IncorrectType));
        }
        return *this;
    }

    /**
     * Test that this Result's field is of a particular type 'T' and has
     * an ID equal to 'id'.
     *
     * Appends an Error::Type::IncorrectType if the field is not of type 'T'.
     * Appends an Error::Type::IncorrectId if the field does not have an ID
     * equal to 'id'.
     *
     * @return itself
     */
    template<typename T>
    Result& is(const std::string& id) {
        T const *s = dynamic_cast<T const *>(f.get());
        if (!s) {
            result = Fail;
            errors.push_back(Error(path, Error::IncorrectType));
        } else if (s->getID() != id) {
            result = Fail;
            errors.push_back(Error(path, Error::IncorrectId));
        }
        return *this;
    }

    /**
     * Test that this Result's field has a subfield with name 'name',
     * apply the function 'fn' to the subfield and, optionally,
     * test that the subfield is of type 'T' (if specified).
     *
     * Appends an Error::Type::IncorrectType if the field is not one of
     * Structure, StructureArray, Union, UnionArray.
     * Appends an Error::Type::IncorrectType if the subfield is not of
     * type 'T' (if 'T' was specified).
     * Appends an Error::Type::MissingField if the subfield is not
     * present.
     *
     * @return itself
     */
    template<Result& (*fn)(Result&), typename T=epics::pvData::Field>
    Result& has(const std::string& name) {
        return has<T>(name, false, fn);
    }

    /**
     * Test that this Result's field has an optional subfield with name
     * 'name' and, if it has, apply the function 'fn' to the subfield and
     * test that the subfield is of type 'T' (if specified).
     *
     * Appends an Error::Type::IncorrectType if the field is not one of
     * Structure, StructureArray, Union, UnionArray.
     * Appends an Error::Type::IncorrectType if the subfield exists and
     * is not of type 'T' (if 'T' was specified).
     *
     * @return itself
     */
    template<Result& (*fn)(Result&), typename T=epics::pvData::Field>
    Result& maybeHas(const std::string& name) {
        return has<T>(name, true, fn);
    }

    /**
     * Test that this Result's field has a subfield with name 'name' and
     * test that the subfield is of type 'T'.
     *
     * Appends an Error::Type::IncorrectType if the field is not one of
     * Structure, StructureArray, Union, UnionArray.
     * Appends an Error::Type::IncorrectType if the subfield is not of
     * type 'T'.
     * Appends an Error::Type::MissingField if the subfield is not
     * present.
     *
     * @return itself
     */
    template<typename T>
    Result& has(const std::string& name) {
        return has<T>(name, false, NULL);
    }

    /**
     * Test that this Result's field has an optional subfield with name
     * 'name' and, if it has, test that the subfield is of type 'T'.
     *
     * Appends an Error::Type::IncorrectType if the field is not one of
     * Structure, StructureArray, Union, UnionArray.
     * Appends an Error::Type::IncorrectType if the subfield exists and
     * is not of type 'T'.
     *
     * @return itself
     */
    template<typename T>
    Result& maybeHas(const std::string& name) {
        return has<T>(name, true, NULL);
    }

    std::ostream& dump(std::ostream& os) const {
        os << "Result(valid=" << (result == Pass) << ", errors=[ ";

        std::vector<Error>::const_iterator it;
        for (it = errors.cbegin(); it != errors.cend(); ++it) {
            (*it).dump(os);
            os << " ";
        }
        os << "])";
        return os;
    }

private:
    template<typename T>
    Result& has(const std::string& name, bool optional, Result& (*check)(Result&) = NULL) {
        epics::pvData::FieldConstPtr field;

        switch(f->getType()) {
            case epics::pvData::Type::structure:
                field = static_cast<epics::pvData::Structure const *>(f.get())->getField(name);
                break;
            case epics::pvData::Type::structureArray:
                field = static_cast<epics::pvData::StructureArray const *>(f.get())->getStructure()->getField(name);
                break;
            case epics::pvData::Type::union_:
                field = static_cast<epics::pvData::Union const *>(f.get())->getField(name);
                break;
            case epics::pvData::Type::unionArray:
                field = static_cast<epics::pvData::UnionArray const *>(f.get())->getUnion()->getField(name);
                break;
            default:
                // Expected a structure-like Field
                result = Fail;
                errors.push_back(Error(path, Error::Type::IncorrectType));
                return *this;
        }

        if (!field) {
            if (!optional) {
                result = Fail;
                errors.push_back(Error(path + "." + name, Error::Type::MissingField));
            }
        } else if (!dynamic_cast<T const *>(field.get())) {
            result = Fail;
            errors.push_back(Error(path + "." + name, Error::Type::IncorrectType));
        } else if (check) {
            Result r(field, path + "." + name);
            *this |= check(r);
        }

        return *this;
    }
};
}}

#endif