�~��e��ǎ([�]�q�������߭V5�=R�5��B�,�i�dϫc_�f�a���	sf&�X��pf�X2�ǉ��D�Kd�Y��=���N=�����}���*1�G�g�w�e�2�b
�.}��"_�|���oC�V�o��F�'��[#���4K�Q��/#��������A�������Z&����|IZ2��h�
��@�q
,Q�@���)��:�
�K�~#&��.P�F�y
|GT/m
hq�(��#��@��D�9� ������ѯ\&k��E
�k/Qx��5$���p^��2ܠ�2�^�����L�y
g)<M����$�Y	�]����z�"��Hp�S�L?o�?*Px��=�(�Yzi�
�i��?�~�8R��5�8Mr��H�/���,i��1��N�4Yyb�����u��%k��U�V$[$2>d�dդ�#k
�7L�2A��dm�U"�Ys�[���>�K���$2Y3d����W����d-�����d�Y�Y�Hև���ҧ2��c���䝁��������_R{�/d�Rl��TE&�%;�Slv��T�R�*P�;��)�D�J}C�
�[0��>!��R_)���J���6�JM {�����(ń��R_�-�պ� (�)��j�`"�qUJi.���Ô���J�)U�T�R���R���Gw|��T�R���S*O)M�dfMf�\@i�l?J6J9�ܣ����^{;L��\Im�~���#��-3{��ض��d��֌Of�ȶ�A��&)�<�R�	��<�R��2s@��v�����HO��-�q�����?�߿���-Ɏ��5��d�ɎRϜ�R�/����yʅ�$�9h^�Ԧ�dYN��i�rq��PKS.J�	Ծ���������<j�gSn�rl�Z��W9Y���,9�L����Q.E�Q��7�6�Z�jY�}Cm�ra�q�1�,��PA�˞������)ǝk]�� j	Դ�������.��?j�K$:�H��\�r��>4�hN��(g��\F��fͪ���f��nh��ni��Y4Kr�"gVi�Y�Ti��i����k�{Ds�<�,�/�����A�o�����e4=ZJs�i���5��r��	o-5��_��iN�/PF�*ߔ����|QƦ2�2E�"Uؘ�L6fn(�J��2��Wƺb3`eZ��u��&U��Y��5C�h�y�X��Օ1���2�Tϣ�k��e��զ�?�r~�*��5x�+PeN;*�7P#����zрw����$Uf�u"�ϩ�KY-�_+�#���~B�e��e>���GT��˗T��zGY]e5�u@�se������T��:7�Q̑�]��Tֱ�6���_����O����lO��!ei��,v{*�o�7���Iu�Έ�:YӈJM���i(;�쨲Ǖ�����1�v%״ه�a�v��~6#r�Q�)e'�=BM?_�k��U��.��_��'T��(;��1epK�]'Ķ��9�*�T>��	ٸ�vN�c��I�w�.��=oR;K�9j��ƃ�sݭlp:��ʏP{Z�C*W���Uޢ��"_*�YS�Ͽ/P������Ǹ*~S�/�h�q���r�J��8��l�SŴ*Nȝ?K�t�����TŌ*����S������{K��^Q�R-��ҼZZTKe�TTK�@�T��K�!�|��_�j�.���B���\�<�řZ�M�j�U�}����~��ϧj��@m�P�SZ�W�jcCm��-������w�΁��J�jΫ��� �,��ݯ�.��u����W��g�	�ӣ��y�A�T�]�7��Rjo\�%���q���8��i�4�%���$sh�x���{�k��2�����!Ț���n ���#�B��+�4� k�=�6XG�I�5Dg�Bt����P���u$v��X�5$�H��!	�v��2qd�Ȱ��9��������-J��e��r~�|ș��6��
d��4��4�)Jy�ݠݲ�W:٣�c���i�J��)��Q+����&�h���@�Ä���ye*�H���|�/({R�Z_fP����>��%U7UNULL)
		{
			luaL_Reg reg = {NULL, NULL};
			out.push_back(reg);
			return ;
		}
		for (int index = 0; table[index].name != NULL; index++)
		{
			if (!isInTable(out, table[index].name))
				out.push_back(table[index]);
		}
		if (self->super() == NULL)
			getFunctionsInTable(self->super(), out, NULL);
		else if (table == self->getMethodTable())
			getFunctionsInTable(self->super(), out, self->super()->getMethodTable());
		else
			getFunctionsInTable(self->super(), out, self->super()->getFunctionTable());
	}
#ifndef _LUA_5_2_
#   define lua_pushglobaltable(state) lua_settable(state, LUA_GLOBALSINDEX)
#endif
public:
    
	
    static int expose(lua_State* state)
	{
		std::vector<luaL_Reg> functions;
		std::vector<luaL_Reg> methods;

        BClass<T>* c = new BClass<T>();
		getFunctionsInTable(c, functions, (luaL_Reg*)_functions);
		getFunctionsInTable(c, methods, (luaL_Reg*)_methods);
		delete c;
        
        // On copie dans le tableau "_className" les methodes de la classe
        luaL_openlib(state, _className, (luaL_Reg*)&functions[0], 0);
        // On recupere l'addresse du tableau sur la Stack
        int Class = lua_gettop(state);
        // On copie d'autre fonction dans le tableau
        luaL_openlib(state, _className, (luaL_Reg*)&methods[0], 0);
		
        // On ecrit : _className.__index = _className
        lua_pushstring(state, "__index");
		lua_pushvalue(state, -2);
		lua_settable(state, -3);
        
        /*
         *  local B_MT_FOR__className = {}
         *  B_MT_FOR__className.__call = newInstance
         */
        BString MTClassname = BString("B_MT_FOR_") + _className;
        luaL_newmetatable(state, MTClassname.c_str());
        lua_pushcfunction(state, newInstance);
        lua_setfield(state, -2, "__call");
        
        /*
         *  setmetatable(_className, B_MT_FOR__className)
         */
        lua_pushvalue(state, Class);
        luaL_getmetatable(state, MTClassname.c_str());
        lua_setmetatable(state, -2);
        lua_pushvalue(state, Class);
        /*
         *  on garde l'addresse de la classe dans les registres Lua pour la retrouver a l' instantiation et declarer la class comme une metatable standard
         */
        lua_setfield(state, LUA_REGISTRYINDEX, _className);  /* registry.name = metatable */
		return 1;
	}
    
	static int newInstance(lua_State* state, T* cpy)
	{
		BObject* instance;
		if (cpy)
			instance = cpy;
		else
			instance = new T();
		if (instance)
		{
			if (!cpy) {
				instance->init(state);
            }
			instance->___lua_instance();
		}
        
		T** ptr = (T **)lua_newuserdata(state, sizeof(T*));
		*ptr = (T*)instance;
        luaL_getmetatable(state, _className);
		lua_setmetatable(state, -2);
		return 1;
	}
    
	static int newInstance(lua_State* state)
	{
        lua_remove(state, -lua_gettop(state));
		return newInstance(state, NULL);
	}

	static int pushObject(lua_State* state, BObject* object)
	{
		BObject** ptr = (BObject **)lua_newuserdata(state, sizeof(BObject*));
		*ptr = object;
        object->___lua_instance();
		luaL_getmetatable(state, _className);
		lua_setmetatable(state, -2);

		return 1;
	}

	static T *self(lua_State* state, int index = 1)
	{
	  void *ud = lua_touserdata(state, index);
	  luaL_argcheck(state, ud != NULL, 1, std::string(std::string("'") + std::string(_className) + std::string("' expected")).c_str());
	  return dynamic_cast<T *>(*(T**)ud);
	}

	virtual const struct luaL_Reg* getFunctionTable() const
	{
	 return _functions;
	}

	virtual const struct luaL_Reg* getMethodTable() const
	{
	 return _methods;
	}

	virtual const char* getClassName() const
	{
	 return _className;
	}

	virtual const BMetaClass* super() const
	{
	 return _super;
	}
};

#endif /* BCLASSEXPOSER_H_ */
