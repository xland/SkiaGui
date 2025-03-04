每个组件都有一个Re-usable "components"方法
Re-usable "components"方法是支持子组件的
初始化和改变窗口大小时才会Clay_BeginLayout，Clay_EndLayout
只有Clay_EndLayout方法才能得到Clay_RenderCommandArray 
只有可见元素才会出现在Clay_RenderCommandArray 数组内
处理WM_PAINT时，遍历Clay_RenderCommandArray 数组
	与上次renderCommand对比，判断是否为dirty
	根据renderCommand->commandType绘图
Clay_OnHover有回调函数
提供测量文本的函数


提供一个layout方法
	执行Clay_BeginLayout，Clay_EndLayout
	得到Clay_RenderCommandArray 

窗口大小改变时，执行layout方法

图像用不了
生成的renderCommand好像也没啥用

