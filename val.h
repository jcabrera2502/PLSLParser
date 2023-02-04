#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>

using namespace std;

enum ValType { VINT, VREAL, VSTRING, VBOOL, VERR };

class Value {
    ValType	T;
    bool    Btemp;
	int		Itemp;
	float   Rtemp;
    string	Stemp;
       
public:
    Value() : T(VERR),  Btemp(false), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(bool vb) : T(VBOOL), Btemp(vb), Itemp(0), Rtemp(0.0), Stemp("") {}
    Value(int vi) : T(VINT), Btemp(false), Itemp(vi), Rtemp(0.0), Stemp("") {}
	Value(float vr) : T(VREAL), Btemp(false), Itemp(0), Rtemp(vr), Stemp("")  {}
    Value(string vs) : T(VSTRING), Btemp(false), Itemp(0), Rtemp(0.0), Stemp(vs) {}
    
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsString() const { return T == VSTRING; }
    bool IsReal() const {return T == VREAL;}
    bool IsBool() const {return T == VBOOL;}
    
    int GetInt() const { if( IsInt() ) return Itemp; throw "RUNTIME ERROR: Value not an integer"; }
    
    string GetString() const { if( IsString() ) return Stemp; throw "RUNTIME ERROR: Value not a string"; }
    
    float GetReal() const { if( IsReal() ) return Rtemp; throw "RUNTIME ERROR: Value not an integer"; }
    
    bool GetBool() const {if(IsBool()) return Btemp; throw "RUNTIME ERROR: Value not a boolean";}
    
    void SetType(ValType type)
    {
    	T = type;
	}
	
    void SetInt(int val)
    {
    	Itemp = val;
	}
	
	void SetReal(float val)
    {
    	Rtemp = val;
	}
	
	void SetString(string val)
    {
    	Stemp = val;
	}
	
	void SetBool(bool val)
    {
    	Btemp = val;
	}
	
	
    // add op to this
    Value operator+(const Value& op) const
    {
        if (op.IsInt() && this->IsInt())
        {
            int add = op.Itemp + this->GetInt();
            return Value(add);
        }
        else if (op.IsReal() && this->IsReal())
        {
            float add = op.Rtemp + this->GetReal();
            return Value(add);
        }
        else if (op.IsReal() && this->IsInt())
        {
            float add = op.Rtemp + this->GetInt();
            return Value(add);
        }
        else if (op.IsInt() && this->IsReal())
        {
            float add = op.Itemp + this->GetReal();
            return Value(add);  
        }
        else if (op.IsString() && this->IsString())
        {
            string add =  this->GetString() + op.Stemp;
            return Value(add); 
        }
        else
        {
            return Value();
        }
        return op;
    }
    
    // subtract op from this
    Value operator-(const Value& op) const
    {
        if (op.IsInt() && this->IsInt())
        {
            int add = op.Itemp - this->GetInt();
            return Value(add);
        }
        else if (op.IsReal() && this->IsReal())
        {
            float add = op.Rtemp - this->GetReal();
            return Value(add);
        }
        else if (op.IsReal() && this->IsInt())
        {
            float add = op.Rtemp - this->GetInt();
            return Value(add);
        }
        else if (op.IsInt() && this->IsReal())
        {
            float add = op.Itemp - this->GetReal();
            return Value(add);  
        }
        else
        {
            return Value();
        }
        return op;
    }
    
    // multiply this by op
    Value operator*(const Value& op) const
    {
        if (op.IsInt() && this->IsInt())
        {
            int add = op.Itemp * this->GetInt();
            return Value(add);
        }
        else if (op.IsReal() && this->IsReal())
        {
            float add = op.Rtemp * this->GetReal();
            return Value(add);
        }
        else if (op.IsReal() && this->IsInt())
        {
            float add = op.Rtemp * this->GetInt();
            return Value(add);
        }
        else if (op.IsInt() && this->IsReal())
        {
            float add = op.Itemp * this->GetReal();
            return Value(add);  
        }
        else
        {
            return Value();
        }
        return op;
    }
    
    // divide this by op
    Value operator/(const Value& op) const
    {
        if (op.IsInt() && this->IsInt())
        {
            int add = this->GetInt() / op.Itemp;
            return Value(add);
        }
        else if (op.IsReal() && this->IsReal())
        {
            float add = this->GetReal() / op.Rtemp;
            return Value(add);
        }
        else if (op.IsReal() && this->IsInt())
        {
            float add = this->GetInt() / op.Rtemp;
            return Value(add);
        }
        else if (op.IsInt() && this->IsReal())
        {
            float add = this->GetReal() / op.Itemp;
            return Value(add);  
        }
        else
        {
            return Value();
        }
        return op;
    }
    
    Value operator==(const Value& op) const
    {
        if (op.IsInt() && this->IsInt())
        {
            if (op.Itemp == this->GetInt())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsReal() && this->IsReal())
        {
            if (op.Rtemp == this->GetReal())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsReal() && this->IsInt())
        {
            if (op.Rtemp == this->GetInt())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsInt() && this->IsReal())
        {
            if (op.Itemp == this->GetReal())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsString() && this->IsString())
        {
            if (op.Stemp == this->GetString())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsBool() && this->IsBool())
        {
            if (op.Btemp == this->GetBool())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return Value();
        }
        return op;
    }

	Value operator>(const Value& op) const
    {
        if (op.IsInt() && this->IsInt())
        {
            if (op.Itemp < this->GetInt())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsReal() && this->IsReal())
        {
            if (op.Rtemp < this->GetReal())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsString() && this->IsString())
        {
            if (op.Stemp < this->GetString())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsBool() && this->IsBool())
        {
            if (op.Btemp < this->GetBool())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return Value();
        }
        return op;
    }
	
	Value operator<(const Value& op) const
    {
        if (op.IsInt() && this->IsInt())
        {
            if (op.Itemp > this->GetInt())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsReal() && this->IsReal())
        {
            if (op.Rtemp > this->GetReal())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsString() && this->IsString())
        {
            if (op.Stemp > this->GetString())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (op.IsBool() && this->IsBool())
        {
            if (op.Btemp > this->GetBool())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return Value();
        }
        return op;
    }
	
	    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.Itemp;
        else if( op.IsString() ) out << op.Stemp;
        else if( op.IsReal()) out << fixed << showpoint << setprecision(2) << op.Rtemp;
        else if (op.IsBool()) {
        	if(op.GetBool()) cout << "true" ;
        	else
        		cout << "false" ;
		}
        else out << "ERROR";
        return out;
    }
};


#endif
