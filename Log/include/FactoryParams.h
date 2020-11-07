#ifndef _FACTORYPARAMS_H_
#define _FACTORYPARAMS_H_

#include <map>
#include <string>
#include <sstream>
#include <stdexcept>

namespace log {

class FactoryParams;

class base_validator_data
{
    public:
        base_validator_data(const char* tag, const FactoryParams* params)
            : tag_(tag), params_(params)
        {  }
    protected:
        const char* tag_;
        const FactoryParams* params_;
        
        template<typename T>
        void assign(const std::string& param_value, T& value) const
        {
            assign_impl(param_value, value);
        }

        template<typename T>
        void assign_impl(const std::string& param_value, T& value) const
        {
            std::stringstream s;
            s << param_value;
            s >> value;
        }

        void assign_impl(const std::string& param_value, std::string&  value) const
        {
            value = param_value;
        }

        void throw_error(const char* para_name) const
        {
            std::stringstream s;
            s << "Property " << para_name << " required to configure " << tag_;
            throw std::runtime_error(s.str());
        }
};

class parameter_validator;
class FactoryParams
{
    typedef std::map<std::string, std::string> storage_t;
    storage_t storage_;

public:
    typedef storage_t::const_iterator const_iterator;
    const std::string& operator[](const std::string& v) const
    {
        const_iterator it = storage_.find(v);
        if(it != storage_.end())
        {
            return it->second;
        }
        throw std::invalid_argument("There is no parameter: " + v);
    }

    std::string& operator[](const std::string& v)
    {
        return storage_.at(v);
    }

    const_iterator find(const std::string& v) const 
    {
        return storage_.find(v);
    }

    const_iterator begin() const
    {
        return storage_.begin();
    }

    const_iterator end() const
    {
        return storage_.end();
    }
    
    parameter_validator get_for(const char* tag) const;
};

class optional_params_validator;

class required_params_validator : public base_validator_data
{
public:
    required_params_validator(const char* tag, const FactoryParams* params)
        : base_validator_data(tag, params)
    {}

    template<typename T>
    optional_params_validator optional(const char* param, T& value) const;
    template<typename T>
    const required_params_validator& operator()(const char* param, T& value) const
    {
        FactoryParams::const_iterator it = params_->find(param);
        if(it != params_->end())
            assign(it->second, value);
        else
            throw_error(param);
        return *this;
    }
};

class optional_params_validator : public base_validator_data
{
public:
    optional_params_validator(const char* tag, const FactoryParams* params) 
        : base_validator_data(tag, params) 
    {}

    template<typename T>
    required_params_validator required(const char* param, T& value) const
    {
        required_params_validator v(tag_, params_); 
        v(param, value); 
        return v;  
    }
    
    template<typename T>
    const optional_params_validator& operator()(const char* param, T& value) const
    {
        FactoryParams::const_iterator it = params_->find(param);
        if(it != params_->end())
            assign(it->second, value);
        else
            throw_error(param);
        return *this;
    }
};

class parameter_validator : public base_validator_data
{
    public:
        parameter_validator(const char* tag, const FactoryParams* params)
            : base_validator_data(tag, params) 
        {  }

        template<typename T>
        required_params_validator required(const char* param, T& value) const
        {
            required_params_validator v(tag_, params_);
            v(param, value);
            return v;
        }

        template<typename T>
        optional_params_validator optional(const char* param, T& value) const
        {
            optional_params_validator v(tag_, params_);
            v(param, value);
            return v;
        }

};

inline parameter_validator FactoryParams::get_for(const char* tag) const
{
    return parameter_validator(tag, this);
}

}

#endif
