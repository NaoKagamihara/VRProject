////////////////////////////////////
//

class CGameBase
{
public:

	CGameBase(){};
	virtual ~CGameBase(){};
	
	virtual void Create(){};

	virtual void Initialize(){};

	virtual void Update(){};

	virtual void RearUpdate(){};

	virtual void Render(){};

	virtual void RearRender(){};

	virtual void DebugRender(){};

	virtual void Release(){};
};