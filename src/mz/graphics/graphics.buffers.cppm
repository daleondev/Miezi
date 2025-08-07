module;
#include "mz/core/core.h"
export module mz.graphics.buffers;

import std;
import glm;

import mz.core.logging;
import mz.core.behaviours;

// import mz.graphics;

namespace mz { 

    export enum class ShaderDataType 
    {
        None = 0,
        Bool,
        Float,
        Float2,
        Float3,
        Float4,
        Float2x2,
        Float3x3,
        Float4x4,
        Int,
        Int2,
        Int3,
        Int4,
        Int2x2,
        Int3x3,
        Int4x4
    };

    static std::size_t shaderDataTypeSize(const ShaderDataType type)
    {
        switch(type) {
            case ShaderDataType::Bool:       return sizeof(bool);
            case ShaderDataType::Float:      return sizeof(float);
            case ShaderDataType::Float2:     return sizeof(float)*2;
            case ShaderDataType::Float3:     return sizeof(float)*3;
            case ShaderDataType::Float4:     return sizeof(float)*4;
            case ShaderDataType::Float2x2:   return sizeof(float)*2*2;
            case ShaderDataType::Float3x3:   return sizeof(float)*3*3;
            case ShaderDataType::Float4x4:   return sizeof(float)*4*4;
            case ShaderDataType::Int:        return sizeof(int);
            case ShaderDataType::Int2:       return sizeof(int)*2;
            case ShaderDataType::Int3:       return sizeof(int)*3;
            case ShaderDataType::Int4:       return sizeof(int)*4;
            case ShaderDataType::Int2x2:     return sizeof(int)*2*2;
            case ShaderDataType::Int3x3:     return sizeof(int)*3*3;
            case ShaderDataType::Int4x4:     return sizeof(int)*4*4;
            case ShaderDataType::None:       return 0;
        }

        MZ_ASSERT(false, "Unknown shader data type");
        return 0;
    }

    struct BufferElement 
    {
        ShaderDataType type;
        std::string_view name;     
        std::size_t size;
        std::size_t offset;  
        bool normalized;

        BufferElement() = default;
        BufferElement(ShaderDataType type, const std::string_view& name, bool normalized = false)
            : type(type), name(name), size(shaderDataTypeSize(type)), offset(0), normalized(normalized)
        {       
        } 

        std::size_t getComponentCount() const
        {
            switch(type) {
                case ShaderDataType::Bool:       return 1;
                case ShaderDataType::Float:      return 1;
                case ShaderDataType::Float2:     return 2;
                case ShaderDataType::Float3:     return 3;
                case ShaderDataType::Float4:     return 4;
                case ShaderDataType::Float2x2:   return 2*2;
                case ShaderDataType::Float3x3:   return 3*3;
                case ShaderDataType::Float4x4:   return 4*4;
                case ShaderDataType::Int:        return 1;
                case ShaderDataType::Int2:       return 2;
                case ShaderDataType::Int3:       return 3;
                case ShaderDataType::Int4:       return 4;
                case ShaderDataType::Int2x2:     return 2*2;
                case ShaderDataType::Int3x3:     return 3*3;
                case ShaderDataType::Int4x4:     return 4*4;
                case ShaderDataType::None:       return 0;
            }

            MZ_ASSERT(false, "Unknown shader data type");
            return 0;
        }
    };

    export class BufferLayout : IIterable<BufferElement>
    {
    public:
        typedef std::vector<BufferElement>::iterator BufferLayoutIterator;
        typedef std::vector<BufferElement>::const_iterator BufferLayoutConstIterator;

        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_elements(elements), m_stride(0)
        {     
            calculateOffsetsAndStride();
        }

        inline const std::vector<BufferElement>& getElements() const { return m_elements; }
        inline std::size_t getStride() const { return m_stride; }

        BufferElement* begin() override { return m_elements.data(); }
        BufferElement* end() override { return m_elements.data() + m_elements.size(); }
        const BufferElement* begin() const override { return m_elements.data(); }
        const BufferElement* end() const override { return m_elements.data() + m_elements.size(); }

    private:
        void calculateOffsetsAndStride()
        {
            std::size_t offset = 0;
            m_stride = 0;
            for (auto& element : m_elements) {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }

        std::vector<BufferElement> m_elements;
        std::size_t m_stride;
    };

    export template <typename T>
    class IGraphicsBuffer : public ICastable
    {
    public:
        virtual ~IGraphicsBuffer() = default;

        virtual void bind() const = 0;
        virtual void release() const = 0;

        virtual void allocate(const std::vector<T>& data) = 0;
    };

    //------------------------------------------------------
    //                      VertexBuffer
    //------------------------------------------------------

    export class VertexBufferBase : public IGraphicsBuffer<float>
    {
    public:
        virtual ~VertexBufferBase() = default;

        virtual void setLayout(const BufferLayout& layout) { m_layout = layout; }
        virtual const BufferLayout& getLayout() const { return m_layout; }

    protected:
        BufferLayout m_layout;

    };

    //------------------------------------------------------
    //                      IndexBuffer
    //------------------------------------------------------

    export class IndexBufferBase : public IGraphicsBuffer<std::uint32_t>
    {
    public:
        virtual ~IndexBufferBase() = default;

        virtual std::size_t getCount() { return m_count; }

    protected:
        std::size_t m_count;

    };

    

}