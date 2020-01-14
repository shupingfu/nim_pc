#pragma once
namespace shared
{
	class TemplatedObjectFactoryWrapper;
	/*
	������TemplatedObjectFactory
	���ܣ�����ע�����͵���Ϣ����һ��TOBJFLG��ʵ��������TBaseӦ��TOBJFLG�ĸ���
	*/
	template<typename TBase, typename TOBJFLG>
	class TemplatedObjectFactory : public nbase::Singleton<TemplatedObjectFactory<typename TBase, typename TOBJFLG>>
	{
	private:
		using _ParentType = nbase::Singleton<TemplatedObjectFactory<typename TBase, typename TOBJFLG>>;
		using _MyType = TemplatedObjectFactory<typename TBase, typename TOBJFLG>;
		friend class TemplatedObjectFactoryWrapper;
		SingletonHideConstructor(_MyType);		
	private:
		TemplatedObjectFactory() = default;
		~TemplatedObjectFactory() = default;
	private:		
		/*
		����TClassʵ���ķ���, ����TClass : public TBase�Ĺ�ϵ
		params TClass�Ĺ������
		����TBase���͵�ָ����� 
		*/
		template<typename TClass>
		TBase* Create()
		{
			return dynamic_cast<TBase*>(new TClass);
		}
		template<typename TClass>
		void AddCreateFunction(TOBJFLG flg)
		{			
			auto it = std::find_if(crate_function_list_.begin(), crate_function_list_.end(), [&](const decltype(*crate_function_list_.begin()) & item){
				return flg == item.first;
			});
			if (it == crate_function_list_.end())
				crate_function_list_.emplace_back(std::make_pair(flg, std::bind(&TemplatedObjectFactory::Create<TClass>, this)));
		}
		auto CreateSharedObject(TOBJFLG flg)->std::shared_ptr<TBase>
		{
			auto it = std::find_if(crate_function_list_.begin(), crate_function_list_.end(), [&](const decltype(*crate_function_list_.begin()) & item){
				return flg == item.first;
			});
			if (it != crate_function_list_.end())
				return std::shared_ptr<TBase>((*it).second());
			return nullptr;
		}
		TBase* CreateObject(TOBJFLG flg)
		{
			TBase* ret = nullptr;
			auto it = std::find_if(crate_function_list_.begin(), crate_function_list_.end(), [&](const decltype(*crate_function_list_.begin()) & item){
				return flg == item.first;
			});
			if (it != crate_function_list_.end())
				ret = (*it).second();
			return ret;
		}
		void CreateAllSharedObject(std::list<std::shared_ptr<TBase>>& objects)
		{
			for (auto it : crate_function_list_)
				objects.emplace_back(std::shared_ptr<TBase>(it.second()));		
		}
	private:		
		std::list<std::pair<TOBJFLG, std::function<TBase*()>>> crate_function_list_;
	};	
	class TemplatedObjectFactoryWrapper
	{
	public:
		//ע������
		template<typename TBase, typename TObject, typename TOBJFLG>
		static void RegisteredOjbect(const TOBJFLG& flg)
		{
			using TDecayType = typename std::decay<TOBJFLG>::type;
			if (std::is_base_of<TBase, TObject>::value)
			{
				TemplatedObjectFactory<TBase, TDecayType>::GetInstance()->AddCreateFunction<TObject>(flg);
			}				
		}
		//���������ü�����ʵ��
		template<typename TBase, typename TFLG>
		static auto InstantiateSharedRegisteredOjbect(const TFLG& flag)->std::shared_ptr<TBase>
		{
			using TDecayType = typename std::decay<TFLG>::type;
			return TemplatedObjectFactory<TBase, TDecayType>::GetInstance()->CreateSharedObject(flag);
		}
		//����ʵ��
		template<typename TBase, typename TFLG>
		static auto InstantiateRegisteredOjbect(const TFLG& flag)->TBase*
		{
			using TDecayType = typename std::decay<TFLG>::type;
			return TemplatedObjectFactory<TBase, TDecayType>::GetInstance()->CreateObject(flag);
		}
		//����������ע��Ϊ TBase �� TFLG Ϊ��ʶ�ĵ�ʵ��
		template<typename TBase, typename TFLG>
		static auto InstantiateAllRegisteredSharedOjbect()->std::list<std::shared_ptr<TBase>>
		{
			using TDecayType = typename std::decay<TFLG>::type;
			std::list<std::shared_ptr<TBase>> ret;
			TemplatedObjectFactory<TBase, TDecayType>::GetInstance()->CreateAllSharedObject(ret);				
			return ret;
		}
	};	
}
