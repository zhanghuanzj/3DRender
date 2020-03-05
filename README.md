# 3DRender
**软件光栅渲染器**
## 主要实现了以下功能：
## 使用说明
**Tab：** 线框模型，光栅化模型，纹理模型切换 

**L**：光照开关

**相机变换**: AD控制相机左右移动，WS控制相机前后移动，鼠标按住左键控制相机角度，按住右键控制相机移动，滚轮控制与物体的远近
## 2D部分：

 - 光栅化2D点
 - 光栅化2D直线
 - 光栅化2D三角形
##3D部分：

 - 把顶点从三维世界空间变换至二维屏幕空间，把顶点连线（如各种三维正多面体）光栅化成wire frame模型
 - 三角形光栅化
 - 使用深度缓冲
 - 实现纹理映射，透视纹理校正
 - 实现Blinn-Phong光照
 
 
 ![像素绘制](http://i.imgur.com/QOE5dDf.png)
 
	像素绘制
![三角形光栅化](http://i.imgur.com/UZXC2oK.png)

	三角形光栅化
![相框模型](http://i.imgur.com/qQDIDqA.png)

	线框模型
![](http://i.imgur.com/WJgm3oH.png)

	光栅化cube
![](http://i.imgur.com/CLny0rV.png)

	纹理模型带光照
